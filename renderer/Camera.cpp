#include "Camera.h"


Camera::Camera(XMVECTOR& e, XMVECTOR& d, XMVECTOR& up)
	: Camera(e, d, up, 45.0f, 1.0f, 1.0f, 1000.0f)
{}

Camera::Camera(XMVECTOR& e, XMVECTOR& d, XMVECTOR& up, float fov, float aspect, float nearPlane, float farPlane)
	: position(e), target(d), up(up)
	, fovY(fov), aspect(aspect), nearPlane(nearPlane), farPlane(farPlane)
{
	this->updateCamera();
	this->updatePerspective();
}

Camera::~Camera() {}

void Camera::updateCamera() {
	this->camera = XMMatrixLookAtLH(this->position, this->target, this->up);
}

void Camera::updatePerspective() {
	this->perspective = XMMatrixPerspectiveFovLH(this->fovY, this->aspect, this->nearPlane, this->farPlane);
}

void Camera::move(XMVECTOR& delta) {
	assert(XMVectorGetW(delta) == 0.0f);

	this->position += delta;
	this->target += delta;
	this->updateCamera();
}

void Camera::lookAt(XMVECTOR& point) {
	assert(XMVectorGetW(point) == 1.0f);

	this->target = point;
	this->updateCamera();
}

const XMMATRIX Camera::getCameraInverse() {
	return this->camera;
}

const XMMATRIX Camera::getPerspective() {
	return this->perspective;
}


