#pragma once
#include <iostream>

using namespace std;

class OssNode
{
public:
	string identifier;
	int x_pos;
	int y_pos;
	int z_pos;
	int ratio;
	OssNode() {
		x_pos = -1;
		y_pos = -1;
		z_pos = -1;
		ratio = -1;
	};
	~OssNode() {};
};