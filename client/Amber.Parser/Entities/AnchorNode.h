#pragma once
#include "GenericNode.h"

#include <iostream>

using namespace std;

class AnchorNode : public GenericNode
{
public:
	AnchorNode() {
		cout << "Anchor created!" << endl;
	};
	~AnchorNode() {};
};