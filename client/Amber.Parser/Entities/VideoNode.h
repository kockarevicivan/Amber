#pragma once
#include "GenericNode.h"

#include <iostream>

using namespace std;

class VideoNode : public GenericNode
{
public:
	VideoNode() {
		cout << "Video created!" << endl;
	};
	~VideoNode() {};
};