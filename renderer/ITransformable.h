#pragma once
#include "render-framework.h"

#include "Matrix4.h"

namespace Transmission {

	class FRAMEWORKDLL ITransformable
	{
	public:
		ITransformable();
		~ITransformable();

		virtual Matrix4 getTransform() = 0;
	};

}