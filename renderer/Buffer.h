#pragma once

#include "render-framework.h"

namespace Transmission {

	class FRAMEWORKDLL Buffer
	{
	private:
		size_t num;

	public:
		Buffer(size_t);
		virtual ~Buffer();

		virtual void set() = 0;
		virtual size_t count();
	};
}
