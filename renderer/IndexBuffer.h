#pragma once
#include "Buffer.h"

namespace Transmission {


	class FRAMEWORKDLL IndexBuffer : public Buffer
	{
	public:
		IndexBuffer(size_t);
		~IndexBuffer();

		virtual void set() = 0;
	};

}