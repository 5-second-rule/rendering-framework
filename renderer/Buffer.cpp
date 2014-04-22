#include "Buffer.h"

namespace Transmission {

	Buffer::Buffer(size_t count) : num(count) {}


	Buffer::~Buffer()
	{
	}

	size_t Buffer::count() { return num; }

}