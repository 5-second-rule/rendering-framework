#include "Transformer.h"

#define FLOATMOD(f,m) (((float)(((int)(f)) % (m))) + ((f) - ((int)(f))))

Transformer::Transformer()
{
	this->position = XMVectorSet(0, 0, 0, 1);
	this->rotation = XMVectorSet(0, 0, 0, 0);
	this->scale = XMVectorSet(1, 1, 1, 0);
}


Transformer::~Transformer()
{
}

void Transformer::move(XMVECTOR delta) {
	this->position += delta;
	float x = XMVectorGetX(this->position);
}

void Transformer::rotate(XMVECTOR xyz) {
	this->rotation += xyz;

	float x = XMVectorGetX(this->rotation);
	float y = XMVectorGetY(this->rotation);
	float z = XMVectorGetZ(this->rotation);

	XMVectorSetX(this->rotation, FLOATMOD(x, 360));
	XMVectorSetY(this->rotation, FLOATMOD(y, 360));
	XMVectorSetZ(this->rotation, FLOATMOD(z, 360));
}

void Transformer::setScale(XMVECTOR scale) {
	this->scale = scale;
}

XMMATRIX Transformer::getTransform() {
	XMMATRIX mScale = XMMatrixScaling(XMVectorGetX(this->scale), XMVectorGetY(this->scale), XMVectorGetZ(this->scale));
	XMMATRIX mRotX = XMMatrixRotationX(XMVectorGetX(this->rotation)*XM_PI / 180.0f);
	XMMATRIX mRotY = XMMatrixRotationY(XMVectorGetY(this->rotation)*XM_PI / 180.0f);
	XMMATRIX mRotZ = XMMatrixRotationZ(XMVectorGetZ(this->rotation)*XM_PI / 180.0f);
	XMMATRIX mTrans = XMMatrixTranslation(XMVectorGetX(this->position), XMVectorGetY(this->position), XMVectorGetZ(this->position));
	return mScale * mRotX * mRotY * mRotZ * mTrans;
}
