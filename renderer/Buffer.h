#pragma once

#include "render-framework.h"

namespace Transmission {

	class FRAMEWORKDLL Buffer
	{
	private:
		size_t num;

	public:
		Buffer(size_t size) : num(size) {}
		virtual ~Buffer() = default;

		virtual void set() = 0;

		virtual size_t count() {
			return num;
		}
	};
}
