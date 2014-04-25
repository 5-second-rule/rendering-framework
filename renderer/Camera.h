#pragma once
#include "render-framework.h"

#include "Vector4.h"
#include "Matrix4.h"

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

		void move(Vector4&);
		void lookAt(Vector4&);

		void resize(float newAspect);

		Matrix4 getCameraInverse();
		Matrix4 getPerspective();
		Vector4 getPosition();

	};

}