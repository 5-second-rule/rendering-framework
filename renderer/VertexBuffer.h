#pragma once
#include "Buffer.h"

namespace Transmission {

	class FRAMEWORKDLL VertexBuffer : public Buffer
	{

	public:
		VertexBuffer(size_t);
		~VertexBuffer();

		virtual void set() = 0;
	};

}
