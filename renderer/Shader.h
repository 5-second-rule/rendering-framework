#pragma once
#include "render-framework.h"

namespace Transmission {
	class FRAMEWORKDLL Shader
	{

	public:
		Shader(const char*);
		~Shader();

		virtual void set() = 0;
		virtual void setWithNoLayout() = 0;
	};

}