#pragma once
#include "GenericNode.h"

#include <iostream>

using namespace std;

class AudioNode : public GenericNode
{
public:
	AudioNode() {
		cout << "Audio created!" << endl;
	};
	~AudioNode() {};
};