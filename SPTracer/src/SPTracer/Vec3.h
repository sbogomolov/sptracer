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

		// get vector from spherical coordinates
		static Vec3 FromPhiTheta(float phi, float cosTheta);

		// [] operator
		const float& operator[](size_t index) const;
		float& operator[](size_t index);

		// arithmetic operators
		Vec3 operator+(const Vec3& b) const;
		Vec3 operator-(const Vec3& b) const;
		Vec3 operator*(const Vec3& b) const;
		Vec3 operator/(const Vec3& b) const;

		// op= operators
		Vec3& operator+=(const Vec3& b);
		Vec3& operator-=(const Vec3& b);
		Vec3& operator*=(const Vec3& b);
		Vec3& operator/=(const Vec3& b);

		// arythmetic operators with floats
		Vec3 operator+(float b) const;
		Vec3 operator-(float b) const;
		Vec3 operator*(float b) const;
		Vec3 operator/(float b) const;
		
		// op= operators with floats
		Vec3& operator+=(float b);
		Vec3& operator-=(float b);
		Vec3& operator*=(float b);
		Vec3& operator/=(float b);

		// negation operator
		Vec3 operator-() const;

		// products
		Vec3 Cross(const Vec3& b) const;
		float Dot(const Vec3& b) const;
		
		// reset vector elements
		void Reset();

		// vector length
		float Length() const;

		// normalize vector length
		Vec3 Normalize() const;

		// rotate from one vector to another
		Vec3 RotateFromTo(const Vec3& fromDirection, const Vec3& toDirection) const;

		// rotate from one vector to another about axis
		Vec3 RotateFromTo(const Vec3& fromDirection, const Vec3& toDirection, const Vec3& rotationAxis) const;

		// rotate about axis on angle
		Vec3 RotateAboutAxis(const Vec3& rotationAxis, float cosTheta) const;

		// arythmetic operators with floats when floats come first
		friend Vec3 operator+(float a, const Vec3& b);
		friend Vec3 operator-(float a, const Vec3& b);
		friend Vec3 operator*(float a, const Vec3& b);
		friend Vec3 operator/(float a, const Vec3& b);

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
