#ifndef SPT_VEC3_H
#define SPT_VEC3_H

#include "stdafx.h"

namespace SPTracer
{

	class Vec3
	{
	public:
		Vec3();
		Vec3(float x, float y, float z);

		void Reset();

		void Normalize();
		float EuclideanNorm() const;
		Vec3 RotateFromTo(const Vec3& fromDirection, const Vec3& toDirection);
		Vec3 RotateFromTo(const Vec3& fromDirection, const Vec3& toDirection, const Vec3& rotationAxis);
		Vec3 RotateAboutAxis(const Vec3& rotationAxis, float cosTheta);

		static Vec3 FromPhiTheta(float phi, float cosTheta);
		static Vec3 CrossProduct(const Vec3& lhs, const Vec3& rhs);

		const float& operator[](size_t index) const;
		float& operator[](size_t index);

		friend Vec3 operator+(const Vec3& lhs, const Vec3& rhs);
		friend Vec3 operator+(const Vec3& lhs, const float& rhs);
		friend Vec3 operator+(const float& lhs, const Vec3& rhs);
		friend void operator+=(Vec3& lhs, const Vec3& rhs);
		friend void operator+=(Vec3& lhs, const float& rhs);

		friend Vec3 operator-(const Vec3& rhs);
		friend Vec3 operator-(const Vec3& lhs, const Vec3& rhs);
		friend Vec3 operator-(const Vec3& lhs, const float& rhs);
		friend Vec3 operator-(const float& lhs, const Vec3& rhs);
		friend void operator-=(Vec3& lhs, const Vec3& rhs);
		friend void operator-=(Vec3& lhs, const float& rhs);

		friend float operator*(const Vec3& lhs, const Vec3& rhs);
		friend Vec3 operator*(const Vec3& lhs, const float& rhs);
		friend Vec3 operator*(const float& lhs, const Vec3& rhs);
		friend void operator*=(Vec3& lhs, const float& rhs);

		friend Vec3 operator/(const Vec3& lhs, const float& rhs);
		friend void operator/=(Vec3& lhs, const float& rhs);

	private:
		static const size_t n_ = 3;

#ifdef _MSC_VER
		// alignment for MSVC
		__declspec(align(16)) float values_[n_];
#else
		// alignment for GCC and Clang
		float values_[n_] __attribute__((aligned(16)));
#endif
	};

}

#endif
