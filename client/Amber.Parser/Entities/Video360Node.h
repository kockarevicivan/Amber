#pragma once
#include "GenericNode.h"

#include <iostream>

using namespace std;

class Video360Node : public GenericNode
{
public:
	Video360Node() {
		cout << "Video360 created!" << endl;
	};
	~Video360Node() {};
};