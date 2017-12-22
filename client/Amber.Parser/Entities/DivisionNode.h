#pragma once
#include "GenericNode.h"

#include <iostream>

using namespace std;

class DivisionNode : public GenericNode
{
public:
	DivisionNode() {
		cout << "Division created!" << endl;
	};
	~DivisionNode() {};
};