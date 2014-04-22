#pragma once

#include "render-framework.h"

#include "Vector4.h"

namespace Transmission {

	class FRAMEWORKDLL Matrix4
	{
	private:
		// Utility Methods

		void assign(const float a[4][4]) {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					this->m[j][i] = a[i][j];
		}

		float multRow(const Vector4& v, int row) const {
			const float r[4] = { get(row, 0), get(row, 1), get(row, 2), get(row, 3) };
			return (r[0] * v[0]) + (r[1] * v[1]) + (r[2] * v[2]) + (r[3] * v[3]);
		}

	protected:
		float m[4][4];   // matrix elements

	public:

		// Constructors

		Matrix4() {
			for (int i = 0; i < 4; ++i)
			{
				for (int j = 0; j < 4; ++j)
				{
					m[i][j] = 0;
				}
			}
		}

		Matrix4(
			float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33
			)
		{
			m[0][0] = m00;
			m[0][1] = m01;
			m[0][2] = m02;
			m[0][3] = m03;

			m[1][0] = m10;
			m[1][1] = m11;
			m[1][2] = m12;
			m[1][3] = m13;

			m[2][0] = m20;
			m[2][1] = m21;
			m[2][2] = m22;
			m[2][3] = m23;

			m[3][0] = m30;
			m[3][1] = m31;
			m[3][2] = m32;
			m[3][3] = m33;
		}

		Matrix4(float a[4][4]) {
			this->assign(a);
		}

		float* getPointer() {
			return &m[0][0];
		}

		float get(int row, int col) const {
			return m[col][row];
		}

		void set(int row, int col, float val) {
			m[col][row] = val;
		}

		Matrix4& operator=(Matrix4 rhs) {
			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 4; j++)
					this->m[i][j] = rhs.m[i][j];

			return (*this);
		}
		Matrix4 operator*(const Matrix4& rhs) {
			Matrix4 result;
			result.m[0][0] = this->get(0, 0) * rhs.get(0, 0) + this->get(0, 1) * rhs.get(1, 0) + this->get(0, 2) * rhs.get(2, 0) + this->get(0, 3) * rhs.get(3, 0);
			result.m[0][1] = this->get(0, 0) * rhs.get(0, 1) + this->get(0, 1) * rhs.get(1, 1) + this->get(0, 2) * rhs.get(2, 1) + this->get(0, 3) * rhs.get(3, 1);
			result.m[0][2] = this->get(0, 0) * rhs.get(0, 2) + this->get(0, 1) * rhs.get(1, 2) + this->get(0, 2) * rhs.get(2, 2) + this->get(0, 3) * rhs.get(3, 2);
			result.m[0][3] = this->get(0, 0) * rhs.get(0, 3) + this->get(0, 1) * rhs.get(1, 3) + this->get(0, 2) * rhs.get(2, 3) + this->get(0, 3) * rhs.get(3, 3);
			result.m[1][0] = this->get(1, 0) * rhs.get(0, 0) + this->get(1, 1) * rhs.get(1, 0) + this->get(1, 2) * rhs.get(2, 0) + this->get(1, 3) * rhs.get(3, 0);
			result.m[1][1] = this->get(1, 0) * rhs.get(0, 1) + this->get(1, 1) * rhs.get(1, 1) + this->get(1, 2) * rhs.get(2, 1) + this->get(1, 3) * rhs.get(3, 1);
			result.m[1][2] = this->get(1, 0) * rhs.get(0, 2) + this->get(1, 1) * rhs.get(1, 2) + this->get(1, 2) * rhs.get(2, 2) + this->get(1, 3) * rhs.get(3, 2);
			result.m[1][3] = this->get(1, 0) * rhs.get(0, 3) + this->get(1, 1) * rhs.get(1, 3) + this->get(1, 2) * rhs.get(2, 3) + this->get(1, 3) * rhs.get(3, 3);
			result.m[2][0] = this->get(2, 0) * rhs.get(0, 0) + this->get(2, 1) * rhs.get(1, 0) + this->get(2, 2) * rhs.get(2, 0) + this->get(2, 3) * rhs.get(3, 0);
			result.m[2][1] = this->get(2, 0) * rhs.get(0, 1) + this->get(2, 1) * rhs.get(1, 1) + this->get(2, 2) * rhs.get(2, 1) + this->get(2, 3) * rhs.get(3, 1);
			result.m[2][2] = this->get(2, 0) * rhs.get(0, 2) + this->get(2, 1) * rhs.get(1, 2) + this->get(2, 2) * rhs.get(2, 2) + this->get(2, 3) * rhs.get(3, 2);
			result.m[2][3] = this->get(2, 0) * rhs.get(0, 3) + this->get(2, 1) * rhs.get(1, 3) + this->get(2, 2) * rhs.get(2, 3) + this->get(2, 3) * rhs.get(3, 3);
			result.m[3][0] = this->get(3, 0) * rhs.get(0, 0) + this->get(3, 1) * rhs.get(1, 0) + this->get(3, 2) * rhs.get(2, 0) + this->get(3, 3) * rhs.get(3, 0);
			result.m[3][1] = this->get(3, 0) * rhs.get(0, 1) + this->get(3, 1) * rhs.get(1, 1) + this->get(3, 2) * rhs.get(2, 1) + this->get(3, 3) * rhs.get(3, 1);
			result.m[3][2] = this->get(3, 0) * rhs.get(0, 2) + this->get(3, 1) * rhs.get(1, 2) + this->get(3, 2) * rhs.get(2, 2) + this->get(3, 3) * rhs.get(3, 2);
			result.m[3][3] = this->get(3, 0) * rhs.get(0, 3) + this->get(3, 1) * rhs.get(1, 3) + this->get(3, 2) * rhs.get(2, 3) + this->get(3, 3) * rhs.get(3, 3);


			return result.transpose();
		}

		Vector4 multiply(const Vector4& v) const {

			Vector4 result;
			for (int i = 0; i < 4; i++) {
				result[i] = this->multRow(v, i);
			}

			return result;
		}


		/* Generators */
		static Matrix4 identity() {
			float ident[4][4] = {
					{ 1, 0, 0, 0 },
					{ 0, 1, 0, 0 },
					{ 0, 0, 1, 0 },
					{ 0, 0, 0, 1 }
			};

			return Matrix4(ident);
		}  // create identity matrix

		static Matrix4 rotateX(float angle) {

			float a[4][4] = {
					{ 1, 0, 0, 0 },
					{ 0, cos(angle), -sin(angle), 0 },
					{ 0, sin(angle), cos(angle), 0 },
					{ 0, 0, 0, 1 }
			};

			return Matrix4(a);
		}

		static Matrix4 rotateY(float angle)
		{
			float a[4][4] = {
					{ cos(angle), 0, sin(angle), 0 },
					{ 0, 1, 0, 0 },
					{ -sin(angle), 0, cos(angle), 0 },
					{ 0, 0, 0, 1 }
			};

			return Matrix4(a);
		}
		static Matrix4 rotateZ(float angle)
		{
			float a[4][4] = {
					{ cos(angle), -sin(angle), 0, 0 },
					{ sin(angle), cos(angle), 0, 0 },
					{ 0, 0, 1, 0 },
					{ 0, 0, 0, 1 }
			};

			return Matrix4(a);
		}

		static Matrix4 rotate(const Vector4& v, float angle) {

			if (angle == 0 || isnan(angle)) return Matrix4::identity();

			float a[4][4] = {
					{ cos(angle) + v.x()*v.x()*(1 - cos(angle)), v.x()*v.y()*(1 - cos(angle)) - v.z()*sin(angle), v.x()*v.z()*(1 - cos(angle)) + v.y()*sin(angle), 0 },
					{ v.y()*v.x()*(1 - cos(angle)) + v.z()*sin(angle), cos(angle) + v.y()*v.y()*(1 - cos(angle)), v.y()*v.z()*(1 - cos(angle)) - v.x()*sin(angle), 0 },
					{ v.z()*v.x()*(1 - cos(angle)) - v.y()*sin(angle), v.z()*v.y()*(1 - cos(angle)) + v.x()*sin(angle), cos(angle) + v.z()*v.z()*(1 - cos(angle)), 0 },
					{ 0, 0, 0, 1 }
			};

			return Matrix4(a);
		}

		static Matrix4 translate(float x, float y, float z) {
			float a[4][4] = {
					{ 1, 0, 0, x },
					{ 0, 1, 0, y },
					{ 0, 0, 1, z },
					{ 0, 0, 0, 1 }
			};

			return Matrix4(a);
		}

		static Matrix4 translate(const Vector4& v) {
			return Matrix4::translate(v[0], v[1], v[2]);
		}

		static Matrix4 scale(float s) {
			return Matrix4::scale(s, s, s);
		}
		static Matrix4 scale(float x, float y, float z) {
			float a[4][4] = {
					{ x, 0, 0, 0 },
					{ 0, y, 0, 0 },
					{ 0, 0, z, 0 },
					{ 0, 0, 0, 1 }
			};

			return Matrix4(a);
		}

		void print() {
			for (int i = 0; i < 4; i++) {
				std::cout << "[ ";
				for (int j = 0; j < 4; j++)
					std::cout << this->m[j][i] << ", ";
				std::cout << "]" << std::endl;
			}
		}

		Matrix4 transpose() {
			//because the internal array is tranposed already, this should work.
			return Matrix4(m);
		}

		Matrix4 rtInverse() {
			float a[4][4] = {
					{ get(0, 0), get(1, 0), get(2, 0), 0 },
					{ get(0, 1), get(1, 1), get(2, 1), 0 },
					{ get(0, 2), get(1, 2), get(2, 2), 0 },
					{ 0, 0, 0, 1 }
			};
			return Matrix4(a) * Matrix4::translate(-get(0, 3), -get(1, 3), -get(2, 3));
		}
	};

	inline Vector4 operator*(const Matrix4& lhs, const Vector4& rhs) {
		return lhs.multiply(rhs);
	}

}