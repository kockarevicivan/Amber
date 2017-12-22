
#include <iostream>
#include <string>
#include <regex>
#include <vector>
#include <fstream>

#include "../Entities/OssNode.h"

using namespace std;

vector<string> get_blocks(string input);
vector<string> get_properties(string block, string* identifier);
OssNode map_oss_node(string identifier, vector<string> properties);
void map_values(char* property_name, char* property_value, OssNode* node);

vector<OssNode> parse_oss(string input) {
	vector<OssNode> parsed_nodes;

	input.erase(remove(input.begin(), input.end(), '\n'), input.end());
	input.erase(remove(input.begin(), input.end(), '\r'), input.end());
	input.erase(remove(input.begin(), input.end(), '\t'), input.end());

	vector<string> blocks = get_blocks(input);

	for (int i = 0; i < blocks.size(); i++) {
		string identifier;
		vector<string> properties = get_properties(blocks.at(i), &identifier);

		OssNode node = map_oss_node(identifier, properties);
		parsed_nodes.push_back(node);
	}

	return parsed_nodes;
}

vector<string> get_blocks(string input) {
	vector<string> blocks;
	regex r("[\.#]?[a-zA-Z0-9]+\\{(.*?)\\}");
	sregex_iterator pos(input.begin(), input.end(), r);
	sregex_iterator end;

	for (; pos != end; pos++) {
		blocks.push_back(pos->str(0));
	}

	return blocks;
}

vector<string> get_properties(string block, string* identifier) {
	vector<string> properties;
	regex r("[a-zA-Z0-9_-]+:[a-zA-Z0-9-]+\;");
	char selector[500];

	int i;
	for (i = 0; i < block.length(); i++) {
		if (block[i] == '{') {
			selector[i] = '\0';
			break;
		}
		selector[i] = block[i];
	}

	string selector_string(selector);
	*identifier = selector_string;

	block.erase(0, i + 1);
	block.erase(block.length() - 1, block.length());

	sregex_iterator pos(block.begin(), block.end(), r);
	sregex_iterator end;

	for (; pos != end; pos++) {
		properties.push_back(pos->str(0));
	}

	return properties;
}

OssNode map_oss_node(string identifier, vector<string> properties) {
	OssNode node;
	node.identifier = identifier;

	for (int i = 0; i < properties.size(); i++) {
		char property_name[500];
		char property_value[500];

		properties.at(i).erase(properties.at(i).length() - 1, properties.at(i).length());

		int j;
		for (j = 0; j < properties.at(i).length(); j++) {
			if (properties.at(i)[j] == ':') {
				property_name[j] = '\0';
				break;
			}
			property_name[j] = properties.at(i)[j];
		}

		int l = 0;
		for (int k = j + 1; k < properties.at(i).length(); k++, l++) {
			property_value[l] = properties.at(i)[k];
		}
		property_value[l] = '\0';

		map_values(property_name, property_value, &node);
	}

	return node;
}

void map_values(char* property_name, char* property_value, OssNode* node) {
	if (!strcmp(property_name, "x_pos")) {
		node->x_pos = atoi(property_value);
	}
	else if (!strcmp(property_name, "y_pos")) {
		node->y_pos = atoi(property_value);
	}
	else if (!strcmp(property_name, "z_pos")) {
		node->z_pos = atoi(property_value);
	}
	else if (!strcmp(property_name, "ratio")) {
		node->ratio = atoi(property_value);
	}
}