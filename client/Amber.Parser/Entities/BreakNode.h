#pragma once
#include "GenericNode.h"

#include <iostream>

using namespace std;

class BreakNode : public GenericNode
{
public:
	BreakNode() {
		cout << "Break created!" << endl;
	};
	~BreakNode() {};
};