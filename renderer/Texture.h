#pragma once
#include "render-framework.h"

namespace Transmission {
	class FRAMEWORKDLL Texture
	{

	public:
		virtual ~Texture() = default;
		virtual void set(int slot) = 0;
	};

}