#pragma once
#include "render-framework.h"

#include <DirectXMath.h>
using namespace DirectX;

class FRAMEWORKDLL Camera
{
private:
	XMVECTOR position;
	XMVECTOR target;
	XMVECTOR up;

	float fovY;
	float aspect;
	float nearPlane;
	float farPlane;

	XMMATRIX camera;
	XMMATRIX perspective;

	void updateCamera();
	void updatePerspective();


public:
	Camera(XMVECTOR&, XMVECTOR&, XMVECTOR&);
	Camera(XMVECTOR&, XMVECTOR&, XMVECTOR&, float, float, float, float);
	
	~Camera();

	void move(XMVECTOR&);
	void lookAt(XMVECTOR&);

	const XMMATRIX getCameraInverse();
	const XMMATRIX getPerspective();

};

