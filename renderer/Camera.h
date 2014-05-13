#pragma once
#include "render-framework.h"

#define _EXPORT_COMMON
#include "common/Vector4.h"
#include "common/Matrix4.h"
using namespace Common;

namespace Transmission {

	class FRAMEWORKDLL Camera
	{
	private:
		Vector4 position;
		Vector4 target;
		Vector4 up;

		float fovY;
		float aspect;
		float nearPlane;
		float farPlane;

		Matrix4 camera;
		Matrix4 perspective;

		void updateCamera();
		void updatePerspective();


	public:
		Camera(Vector4&, Vector4&, Vector4&);
		Camera(Vector4&, Vector4&, Vector4&, float, float, float, float);

		~Camera();

		void move(const Vector4&);
		void lookAt(const Vector4&);

		Matrix4 getCameraInverse();
		Matrix4 getPerspective();
		Vector4 getPosition();

	};

}