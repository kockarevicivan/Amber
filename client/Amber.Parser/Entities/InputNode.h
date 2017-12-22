#pragma once
#include "GenericNode.h"

#include <iostream>

using namespace std;

class InputNode : public GenericNode
{
public:
	InputNode() {
		cout << "Input created!" << endl;
	};
	~InputNode() {};
};