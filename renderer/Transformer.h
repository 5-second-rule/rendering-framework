#pragma once
#include "ITransformable.h"
class FRAMEWORKDLL Transformer : public ITransformable
{
private:
	XMVECTOR position;
	XMVECTOR rotation;
	XMVECTOR scale;

public:
	Transformer();
	~Transformer();

	virtual void move(XMVECTOR);
	virtual void rotate(XMVECTOR);
	virtual void setScale(XMVECTOR);

	virtual XMMATRIX getTransform();
};

