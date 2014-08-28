#pragma once
#include "Buffer.h"

namespace Transmission {


	class FRAMEWORKDLL IndexBuffer : public Buffer
	{
	public:
		IndexBuffer(size_t size) : Buffer(size) {}
		virtual ~IndexBuffer() = default;

		virtual void set() = 0;
	};

}