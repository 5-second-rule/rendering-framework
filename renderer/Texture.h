#pragma once
#include "render-framework.h"

namespace Transmission {
	class FRAMEWORKDLL Texture
	{

	public:
		Texture(const char*);
		~Texture();

		virtual void set() = 0;
	};

}