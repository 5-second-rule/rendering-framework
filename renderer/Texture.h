#pragma once
#include "render-framework.h"

class FRAMEWORKDLL Texture
{

public:
	Texture(const char*);
	~Texture();

	virtual void set() = 0;
};

