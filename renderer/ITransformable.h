#pragma once
#include "render-framework.h"

#include <DirectXMath.h>
using namespace DirectX;

class FRAMEWORKDLL ITransformable
{
public:
	ITransformable();
	~ITransformable();

	virtual XMMATRIX getTransform() = 0;
};

