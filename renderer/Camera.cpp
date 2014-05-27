#define _USE_MATH_DEFINES

#include "Camera.h"

#include <cmath>
#include <cassert>

namespace Transmission {

	Camera::Camera(Vector4& e, Vector4& d, Vector4& up)
		: Camera(e, d, up, 45.0f, 1.0f, 1.0f, 1000.0f)
	{}

	Camera::Camera(Vector4& e, Vector4& d, Vector4& up, float fov, float aspect, float nearPlane, float farPlane)
		: position(e), target(d), up(up)
		, fovY(fov), aspect(aspect), nearPlane(nearPlane), farPlane(farPlane)
	{
		this->updateCamera();
		this->updatePerspective();
	}

	Camera::~Camera() {}

	void Camera::updateCamera() {
		Vector4 z = Vector4::normalize(this->target - this->position);
		Vector4 x = Vector4::normalize(Vector4::cross(this->up, z));
		Vector4 y = Vector4::normalize(Vector4::cross(z, x));

		float a[4][4] = {
				{ x[0], y[0], z[0], 0 },
				{ x[1], y[1], z[1], 0 },
				{ x[2], y[2], z[2], 0 },
				{ -x.dot(position), -y.dot(position), -z.dot(position), 1 }
		};

		this->camera = Matrix4(a);
	}

	void Camera::updatePerspective() {
		float yScale = 1.0f / tan(this->fovY / 2.0f);
		float xScale = yScale / this->aspect;
		float farMnear = this->farPlane - this->nearPlane;

		float a[4][4] = {
				{ xScale, 0, 0, 0 },
				{ 0, yScale, 0, 0 },
				{ 0, 0, this->farPlane / farMnear, 1 },
				{ 0, 0, -1.0f*this->nearPlane*this->farPlane / farMnear, 0 }
		};

		this->perspective = Matrix4(a);
	}

	void Camera::resize(float newAspect) {
		this->aspect = newAspect;
		this->updatePerspective();
	}

	void Camera::move(const Vector4& delta) {
		assert(delta.w() == 0.0f);

		this->position += delta;
		this->target += delta;
		this->updateCamera();
	}

	void Camera::set(const Vector4& position, const Vector4& lookAt, const Vector4& up) {
		assert(position.w() == 1.0f);
		assert(lookAt.w() == 1.0f);
		assert(up.w() == 0.0f);

		this->position = position;
		this->target = lookAt;
		this->up = up;
		this->updateCamera();
	}

	void Camera::lookAt(const Vector4& point) {
		assert(point.w() == 1.0f);

		this->target = point;
		this->updateCamera();
	}

	void Camera::setUp(const Vector4& up) {
		assert(up.w() == 0.0f);

		this->up = up;
		this->updateCamera();
	}

	Matrix4 Camera::getCameraInverse() {
		return this->camera;
	}

	Matrix4 Camera::getPerspective() {
		return this->perspective;
	}

	Vector4 Camera::getPosition() {
		return this->position;
	}


}