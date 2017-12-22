#pragma once
#include <vector>
#include "../Libraries/rapidxml.hpp"

using namespace rapidxml;

class GenericNode
{
	public:
		char* nodeId;
		vector<char*> nodeClasses;
		xml_node<>* parent;
		GenericNode() {
			nodeId = new char[256];
		};
		~GenericNode() {};
};