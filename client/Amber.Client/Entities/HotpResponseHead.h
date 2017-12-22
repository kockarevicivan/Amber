#pragma once
#include "HotpStatus.h"
#include <string>

using namespace std;

class HotpResponseHead
{
public:
	HotpStatus status;
	string mime_type;
	int content_length;
};

