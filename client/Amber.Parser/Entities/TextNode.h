#pragma once
#include "GenericNode.h"

#include <iostream>

using namespace std;

class TextNode : public GenericNode
{
public:
	TextNode() {
		cout << "Text created!" << endl;
	};
	~TextNode() {};
};