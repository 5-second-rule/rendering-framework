#pragma once
#include "render-framework.h"

#include "common/Vector4.h"
#include "common/Matrix4.h"
using namespace Common;

namespace Transmission {

	class FRAMEWORKDLL ITransformable
	{
	public:
		ITransformable();
		~ITransformable();

		virtual Matrix4 getTransform() = 0;
	};

}