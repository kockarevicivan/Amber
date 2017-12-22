#pragma once
#include <string>

#include "HotpResponseHead.h"
#include "HotpResponseBody.h"

using namespace std;

class HotpResponse
{
public:
	HotpResponseHead head;
	HotpResponseBody body;
};

