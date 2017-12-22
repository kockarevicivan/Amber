#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>

#include "../Libraries/picojson.h"
#include "../Entities/HotpResponse.h"
#include "../Entities/HotpResponseHead.h"
#include "../Entities/HotpResponseBody.h"
#include "../Entities/HotpStatus.h"
#include "../../Amber.Parser/Entities/OssNode.h"
#include "../../Amber.Parser/Logic/OssParser.h"

#include "../../Amber.Parser/Logic/XMLParser.h"

// Need to link with Ws2_32.lib, Mswsock.lib, and Advapi32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")

#define _WIN32_WINNT 0x501
#define DEFAULT_BUFLEN 512
#define DEFAULT_IP "127.0.0.1"
#define DEFAULT_PORT "60000"
#define RECV_BUF_SIZE 2048

using namespace std;
using namespace picojson;

HotpResponse ExtractJson(const char* json);
vector<GenericNode*> current_homl_nodes;
vector<char*> current_homl_requests;
vector<GenericNode*> request_page(char* request);
void pull_resources();

WSADATA wsaData;
SOCKET ConnectSocket = INVALID_SOCKET;
struct addrinfo* result = NULL, *ptr = NULL, hints;
char recvbuf[RECV_BUF_SIZE];
int iResult;

void run_client() {
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// Resolve the server address and port
	iResult = getaddrinfo(DEFAULT_IP, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("Getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return;
	}

	// Attempt to connect to an address until one succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

		if (ConnectSocket == INVALID_SOCKET) {
			printf("Socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return;
		}

		// Connect to server.
		iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);

		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}

		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		WSACleanup();
		return;
	}
}

HotpResponse ExtractJson(const char* json) {
	value v;
	string err = "";
	HotpResponse response;

	const char* json_end = parse(v, json, json + strlen(json), &err);

	if (!err.empty()) {
		std::cerr << err << std::endl;
		return response;
	}

	if (v.is<object>()) {
		const value::object& obj = v.get<object>();

		for (value::object::const_iterator i = obj.begin(); i != obj.end(); ++i) {
			value temp = i->second;

			if (temp.is<object>()) {
				const value::object& objTemp = temp.get<object>();
				for (value::object::const_iterator j = objTemp.begin(); j != objTemp.end(); ++j) {

					if (j->first == "_Content") {
						response.body.content = j->second.to_str();
					}
					else if (j->first == "_Status") {
						response.head.status = (HotpStatus)stoi(j->second.to_str());
					}
					else if (j->first == "_ContentLength") {
						response.head.content_length = stoi(j->second.to_str());
					}
					else if (j->first == "_MimeType") {
						response.head.mime_type = j->second.to_str();
					}
				}
			}
		}
	}

	return response;
}

vector<GenericNode*> request_page(char* request) {
	// Send an initial buffer
	iResult = send(ConnectSocket, "", (int)strlen(""), 0);
	if (iResult == SOCKET_ERROR) {
		printf("Initial send failed with error: %d\n", WSAGetLastError());
		closesocket(ConnectSocket);
		WSACleanup();
		return current_homl_nodes;
	}

	iResult = send(ConnectSocket, request, (int)strlen(request), 0);
	iResult = recv(ConnectSocket, recvbuf, RECV_BUF_SIZE, 0);
	if (iResult > 0) {
		HotpResponse response = ExtractJson(recvbuf);

		if (!strcmp(response.head.mime_type.c_str(), "text/homl")) {
			char* xml_to_parse = _strdup(response.body.content.c_str());
			current_homl_nodes = parse_xml(xml_to_parse);
			current_homl_requests = resource_requests;

			ofstream myfile;
			myfile.open("localstorage/index.homl");
			myfile << response.body.content;
			myfile.close();

			//go through each node and request everything with href or src attribute
			pull_resources();
		}
	}
	else if (iResult == 0)
		printf("Connection closed\n");
	else
		printf("recv failed with error: %d\n", WSAGetLastError());

	closesocket(ConnectSocket);
	WSACleanup();

	return current_homl_nodes;
}

void pull_resources() {
	for (int i = 0; i < current_homl_requests.size(); i++) {
		char* request = current_homl_requests[i];
		iResult = send(ConnectSocket, request, (int)strlen(request), 0);
		iResult = recv(ConnectSocket, recvbuf, RECV_BUF_SIZE, 0);
		if (iResult > 0) {
			HotpResponse response = ExtractJson(recvbuf);//Gets the response from json string.

			if (!strcmp(response.head.mime_type.c_str(), "text/oss")) {
				char* oss_to_parse = _strdup(response.body.content.c_str());
				vector<OssNode> oss_nodes = parse_oss(oss_to_parse);

				ofstream myfile;
				//should extract the name from request, or receive it by response
				myfile.open("localstorage/index.oss", std::ios_base::app);
				myfile << "\n" + response.body.content;

				//homl and oss merge logic
				for (int i = 0; i < current_homl_nodes.size(); i++) {
					for (int j = 0; j < oss_nodes.size(); j++) {
						if (ObjectNode* p = static_cast<ObjectNode*>(current_homl_nodes[i])) {

							ObjectNode* pp = (ObjectNode*)current_homl_nodes[i];
							current_homl_nodes.erase(current_homl_nodes.begin() + i);

							if (oss_nodes[j].identifier.at(0) == '#') {//id
								char* temp = pp->nodeId;
								if (!strcmp(oss_nodes[j].identifier.c_str(), temp)) {//compare if the ids match

									pp->pos_x = oss_nodes[j].x_pos;
									pp->pos_y = oss_nodes[j].y_pos;
									pp->pos_z = oss_nodes[j].z_pos;
									pp->ratio = oss_nodes[j].ratio;
								}
							}
							else if (oss_nodes[j].identifier.at(0) == '.') {//class
								if (find(pp->nodeClasses.begin(),
									pp->nodeClasses.end(),
									oss_nodes[j].identifier.c_str()) != pp->nodeClasses.end()) {//compare if the ides match
																												   ///TODO transfer data from oss
								}
							}
							else {
								///TODO element identifier
							}

							current_homl_nodes.push_back(pp);
						}
					}
				}
			}
			else if (!strcmp(response.head.mime_type.c_str(), "application/obj")) {
				ofstream myfile;
				myfile.open("localstorage/test.obj");
				myfile << response.body.content;
				myfile.close();
			}
		}
		else if (iResult == 0) {
			printf("Connection closed\n");
			closesocket(ConnectSocket);
			WSACleanup();
		}
		else
			printf("Recv failed with error: %d\n", WSAGetLastError());
	}
}