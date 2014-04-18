#define _USE_MATH_DEFINES
#include "Transformer.h"

#include <cmath>

#include "util.h"

#define FLOATMOD(f,m) (((float)(((int)(f)) % (m))) + ((f) - ((int)(f))))

Transformer::Transformer()
{
	this->position = Point(0.0f, 0.0f, 0.0f);
	this->rotation = Vector4(0, 0, 0);
	this->scale = Vector4(1, 1, 1);
}


Transformer::~Transformer()
{
}

void Transformer::move(const Vector4& delta) {
	this->position += delta;
}

void Transformer::rotate(const Vector4& xyz) {
	this->rotation += xyz;
	this->rotation.set(
		FLOATMOD(this->rotation.x(), 360),
		FLOATMOD(this->rotation.y(), 360),
		FLOATMOD(this->rotation.z(), 360)
	);
}

void Transformer::setScale(const Vector4& scale) {
	this->scale = scale;
}

Matrix4 Transformer::getTransform() {
	Matrix4 mScale = Matrix4::scale(this->scale.x(), this->scale.y(), this->scale.z());
	Matrix4 mRotX = Matrix4::rotateX(this->rotation.x() * (float)M_PI / 180.0f);
	Matrix4 mRotY = Matrix4::rotateY(this->rotation.y() * (float)M_PI / 180.0f);
	Matrix4 mRotZ = Matrix4::rotateZ(this->rotation.z() * (float)M_PI / 180.0f);
	Matrix4 mTrans = Matrix4::translate(this->position);
	return mScale * mRotX * mRotY * mRotZ * mTrans;
}
