#pragma once
#include "ITransformable.h"


namespace Transmission {

	class FRAMEWORKDLL Transformer : public ITransformable
	{
	private:
		Vector4 position;
		Vector4 rotation;
		Vector4 scale;

	public:
		Transformer();
		~Transformer();

		virtual void move(const Vector4&);
		virtual void rotate(const Vector4&);
		virtual void setScale(const Vector4&);
		virtual void setPosition(const Vector4&);

		virtual Matrix4 getTransform();
	};
}
