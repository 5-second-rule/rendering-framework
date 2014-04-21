#pragma once
#include "render-framework.h"

class FRAMEWORKDLL Input
{
public:
	bool forward;
	bool up, down;
	bool left, right;
	bool rollLeft, rollRight;

	Input();
	~Input();
};
