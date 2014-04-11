#pragma once
#include "Buffer.h"
class FRAMEWORKDLL VertexBuffer : public Buffer
{

public:
	VertexBuffer(size_t);
	~VertexBuffer();

	virtual void set() = 0;
};

