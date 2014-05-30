#pragma once
#include "render-framework.h"

namespace Transmission {
	class FRAMEWORKDLL Texture
	{

	public:
		Texture(const char*);
		virtual ~Texture();

		virtual void set(int slot) = 0;
	};

}