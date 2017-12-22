#pragma once
#include "GenericNode.h"

#include <iostream>

using namespace std;

class ImageNode : public GenericNode
{
public:
	ImageNode() {
		cout << "Image created!" << endl;
	};
	~ImageNode() {};
};