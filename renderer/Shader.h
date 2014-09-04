#pragma once
#include "render-framework.h"

namespace Transmission {
	class FRAMEWORKDLL Shader
	{

	public:
		virtual ~Shader() = default;

		virtual void set() = 0;
		virtual void setWithNoLayout() = 0;
	};

	class FRAMEWORKDLL VertexShader : public Shader {};
	class FRAMEWORKDLL PixelShader : public Shader {};

}