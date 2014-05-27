#pragma once
#include "ITransformable.h"
#include "common/IMoveable.h"


namespace Transmission {

	class FRAMEWORKDLL Transformer : public ITransformable, public IMoveable
	{
	private:
		Vector4 position;
		Matrix4 rotation;
		float scale;

	public:
		Transformer();
		~Transformer();

		/* Relative */
		void move(float dx, float dy, float dz);
		void move(const Vector4& delta);

		void rotate(float x, float y, float z);
		void rotate(const Vector4& vector, float degrees);

		/* Absolute */
		void setScale(float scale);
		
		void setPosition(float x, float y, float z);
		void setPosition(const Vector4& pos);

		void setRotation(float x, float y, float z);

		/* Getters */
		Matrix4 getTransform();
		const Vector4& getPosition() const;
		const Matrix4& getRotation() const;
		const float getScale() const;
	};
}
