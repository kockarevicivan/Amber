#pragma once
#include "GenericNode.h"

#include <iostream>

using namespace std;

class Image360Node : public GenericNode
{
public:
	Image360Node() {
		cout << "Image360 created!" << endl;
	};
	~Image360Node() {};
};