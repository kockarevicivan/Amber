#include <iostream>
#include <vector>

#include "../Libraries/rapidxml.hpp"
#include "../Libraries/rapidxml_utils.hpp"
#include "../Libraries/rapidxml_iterators.hpp"
#include "../Libraries/rapidxml_print.hpp"

#include "../Entities/GenericNode.h"
#include "../Entities/ObjectNode.h"
#include "../Entities/VideoNode.h"
#include "../Entities/Video360Node.h"
#include "../Entities/ImageNode.h"
#include "../Entities/Image360Node.h"
#include "../Entities/AudioNode.h"
#include "../Entities/TextNode.h"
#include "../Entities/BreakNode.h"
#include "../Entities/InputNode.h"
#include "../Entities/AnchorNode.h"
#include "../Entities/DivisionNode.h"

using namespace std;
using namespace rapidxml;

void get_nodes(xml_node<>* node);
void print_list();

vector<GenericNode*> nodeList;
vector<char*> resource_requests;

vector<GenericNode*> parse_xml(char* xml)
{
	xml_document<> doc;
	doc.parse<0>(xml);

	xml_node<>* node = doc.first_node();
	get_nodes(node);

	for (int i = 0; i < nodeList.size(); i++) {
		if (ObjectNode* p = static_cast<ObjectNode*>(nodeList[i])) {
			p->nodeId;
			p->pos_x;
			p->pos_y;
			p->pos_z;
			p->ratio;
		}
	}

	return nodeList;
}

void get_nodes(xml_node<>* node) {

	if (!(node->first_node())) {
		return;
	}

	for (xml_node<>* n = node->first_node(); n; n = n->next_sibling()) {

		if (!strcmp(n->name(), "object")) {
			ObjectNode* objectNode = new ObjectNode();

			for (xml_attribute<> *attr = n->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (!strcmp(attr->name(), "src")) {
					resource_requests.push_back(attr->value());
					strcpy_s(objectNode->src, 257, attr->value());
				}

				if (!strcmp(attr->name(), "id")) {
					char temp[256];
					strcpy_s(temp, sizeof temp, "#");
					strcat_s(temp, sizeof temp, attr->value());
					strcpy_s(objectNode->nodeId, 257, temp);
				}
			}

			nodeList.push_back(objectNode);
		}
		else if (!strcmp(n->name(), "video")) {
			VideoNode videoNode;

			for (xml_attribute<> *attr = n->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (!strcmp(attr->name(), "src")) {
					resource_requests.push_back(attr->value());
				}

				if (!strcmp(attr->name(), "id")) {
					strcpy_s(videoNode.nodeId, sizeof(videoNode.nodeId) + 1, attr->value());
				}
			}

			nodeList.push_back(&videoNode);
		}
		else if (!strcmp(n->name(), "video3d")) {
			Video360Node video360Node;

			for (xml_attribute<> *attr = n->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (!strcmp(attr->name(), "src")) {
					resource_requests.push_back(attr->value());
				}

				if (!strcmp(attr->name(), "id")) {
					strcpy_s(video360Node.nodeId, sizeof(video360Node.nodeId) + 1, attr->value());
				}
			}

			nodeList.push_back(&video360Node);
		}
		else if (!strcmp(n->name(), "img")) {
			ImageNode imageNode;

			for (xml_attribute<> *attr = n->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (!strcmp(attr->name(), "src")) {
					resource_requests.push_back(attr->value());
				}

				if (!strcmp(attr->name(), "id")) {
					strcpy_s(imageNode.nodeId, sizeof(imageNode.nodeId) + 1, attr->value());
				}
			}

			nodeList.push_back(&imageNode);
		}
		else if (!strcmp(n->name(), "img3d")) {
			Image360Node image360Node;

			for (xml_attribute<> *attr = n->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (!strcmp(attr->name(), "src")) {
					resource_requests.push_back(attr->value());
				}

				if (!strcmp(attr->name(), "id")) {
					strcpy_s(image360Node.nodeId, sizeof(image360Node.nodeId) + 1, attr->value());
				}
			}

			nodeList.push_back(&image360Node);
		}
		else if (!strcmp(n->name(), "audio")) {
			AudioNode audioNode;

			for (xml_attribute<> *attr = n->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (!strcmp(attr->name(), "src")) {
					resource_requests.push_back(attr->value());
				}

				if (!strcmp(attr->name(), "id")) {
					strcpy_s(audioNode.nodeId, sizeof(audioNode.nodeId) + 1, attr->value());
				}
			}

			nodeList.push_back(&audioNode);
		}
		else if (!strcmp(n->name(), "txt")) {
			TextNode textNode;

			for (xml_attribute<> *attr = n->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (!strcmp(attr->name(), "id")) {
					strcpy_s(textNode.nodeId, sizeof(textNode.nodeId) + 1, attr->value());
				}
			}

			nodeList.push_back(&textNode);
		}
		else if (!strcmp(n->name(), "br")) {
			BreakNode breakNode;

			for (xml_attribute<> *attr = n->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (!strcmp(attr->name(), "id")) {
					strcpy_s(breakNode.nodeId, sizeof(breakNode.nodeId) + 1, attr->value());
				}
			}

			nodeList.push_back(&breakNode);
		}
		else if (!strcmp(n->name(), "input")) {
			InputNode inputNode;

			for (xml_attribute<> *attr = n->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (!strcmp(attr->name(), "id")) {
					strcpy_s(inputNode.nodeId, sizeof(inputNode.nodeId) + 1, attr->value());
				}
			}

			nodeList.push_back(&inputNode);
		}
		else if (!strcmp(n->name(), "a")) {
			AnchorNode anchorNode;

			for (xml_attribute<> *attr = n->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (!strcmp(attr->name(), "id")) {
					strcpy_s(anchorNode.nodeId, sizeof(anchorNode.nodeId) + 1, attr->value());
				}
			}

			nodeList.push_back(&anchorNode);
		}
		else if (!strcmp(n->name(), "div")) {
			DivisionNode divisionNode;

			for (xml_attribute<> *attr = n->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (!strcmp(attr->name(), "id")) {
					strcpy_s(divisionNode.nodeId, sizeof(divisionNode.nodeId) + 1, attr->value());
				}
			}

			nodeList.push_back(&divisionNode);
		}
		else if (!strcmp(n->name(), "link")) {
			for (xml_attribute<> *attr = n->first_attribute(); attr; attr = attr->next_attribute())
			{
				if (!strcmp(attr->name(), "href")) {
					resource_requests.push_back(attr->value());
				}
			}
		}

		get_nodes(n);
	}
}