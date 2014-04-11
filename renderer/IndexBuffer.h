#pragma once
#include "Buffer.h"

class FRAMEWORKDLL IndexBuffer : public Buffer
{
public:
	IndexBuffer(size_t);
	~IndexBuffer();

	virtual void set() = 0;
};

