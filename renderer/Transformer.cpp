#define _USE_MATH_DEFINES
#include "Transformer.h"

#include <cmath>
#include <cassert>

#include "util.h"

#define FLOATMOD(f,m) (((float)(((int)(f)) % (m))) + ((f) - ((int)(f))))

namespace Transmission {

	Transformer::Transformer()
	{
		this->position = Point(0.0f, 0.0f, 0.0f);
		this->rotation = Matrix4::identity();
		this->scale = 1.0f;
	}


	Transformer::~Transformer() {}

	void Transformer::move(float dx, float dy, float dz) {
		this->move(Vector(dx, dy, dz));
	}

	void Transformer::move(const Vector4& delta) {
		assert(delta.w() == 0);
		this->position += delta;
	}

	void Transformer::rotate(float x, float y, float z) {
		if (x != 0) {
			this->rotation = Matrix4::rotateX(x) * this->rotation;
		}

		if (y != 0) {
			this->rotation = Matrix4::rotateY(y) * this->rotation;
		}

		if (z != 0) {
			this->rotation = Matrix4::rotateZ(z) * this->rotation;
		}
	}

	void Transformer::rotate(const Vector4& vector, float angle) {
		if (angle != 0) {
			this->rotation = Matrix4::rotate(vector, angle) * this->rotation;
		}
	}


	void Transformer::setScale(float scale) {
		this->scale = scale;
	}

	void Transformer::setPosition(float x, float y, float z) {
		this->position = Point(x, y, z);
	}

	void Transformer::setPosition(const Vector4& pos) {
		//assert(pos.w() == 1);
		this->position = pos;
	}

	void Transformer::setRotation(float x, float y, float z) {
		this->rotation = Matrix4::identity();
		this->rotate(x, y, z);
	}

	Matrix4 Transformer::getTransform() {
		Matrix4 mScale = Matrix4::scale(this->scale);
		Matrix4 mTrans = Matrix4::translate(this->position);
		return mTrans * mScale * this->rotation;
	}

	const Vector4& Transformer::getPosition() const {
		return this->position;
	}

	const Matrix4& Transformer::getRotation() const {
		return this->rotation;
	}

	const float Transformer::getScale() const {
		return this->scale;
	}
}