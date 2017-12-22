#pragma once
#include "GenericNode.h"

#include <iostream>

using namespace std;

class ObjectNode : public GenericNode
{
	public:
		int pos_x;
		int pos_y;
		int pos_z;
		int ratio;
		char* src;
		ObjectNode() {
			cout << "Object created!" << endl;
			src = new char[256];
		};
		~ObjectNode() {};
};