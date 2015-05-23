#include "stdafx.h"
#include "Util.h"
#include "Vec3.h"

namespace SPTracer
{

	Vec3::Vec3()
	{
	}

	Vec3::Vec3(float x, float y, float z)
		: values_{x, y, z}
	{
	}

	// get vector from spherical coordinates
	Vec3 Vec3::FromPhiTheta(float phi, float cosTheta)
	{
		const float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);

		return Vec3(
			sinTheta * std::cos(phi),
			sinTheta * std::sin(phi),
			cosTheta
			);
	}


	//
	// [] operator
	//

	const float& Vec3::operator[](size_t index) const
	{
		return values_[index];
	}

	float& Vec3::operator[](size_t index)
	{
		return values_[index];
	}


	//
	// algebraic operators
	//

	Vec3 Vec3::operator+(const Vec3& b) const
	{
		return Vec3(
			values_[0] + b.values_[0],
			values_[1] + b.values_[1],
			values_[2] + b.values_[2]
			);
	}

	Vec3 Vec3::operator-(const Vec3& b) const
	{
		return Vec3(
			values_[0] - b.values_[0],
			values_[1] - b.values_[1],
			values_[2] - b.values_[2]
			);
	}

	Vec3 Vec3::operator*(const Vec3& b) const
	{
		return Vec3(
			values_[0] * b.values_[0],
			values_[1] * b.values_[1],
			values_[2] * b.values_[2]
			);
	}

	Vec3 Vec3::operator/(const Vec3& b) const
	{
		return Vec3(
			values_[0] / b.values_[0],
			values_[1] / b.values_[1],
			values_[2] / b.values_[2]
			);
	}


	//
	// op= operators
	//

	Vec3& Vec3::operator+=(const Vec3& b)
	{
		values_[0] += b.values_[0];
		values_[1] += b.values_[1];
		values_[2] += b.values_[2];

		return *this;
	}

	Vec3& Vec3::operator-=(const Vec3& b)
	{
		values_[0] -= b.values_[0];
		values_[1] -= b.values_[1];
		values_[2] -= b.values_[2];

		return *this;
	}

	Vec3& Vec3::operator*=(const Vec3& b)
	{
		values_[0] *= b.values_[0];
		values_[1] *= b.values_[1];
		values_[2] *= b.values_[2];

		return *this;
	}

	Vec3& Vec3::operator/=(const Vec3& b)
	{
		values_[0] /= b.values_[0];
		values_[1] /= b.values_[1];
		values_[2] /= b.values_[2];

		return *this;
	}


	//
	// algebraic operators with floats
	//

	Vec3 Vec3::operator+(float b) const
	{
		return Vec3(
			values_[0] + b,
			values_[1] + b,
			values_[2] + b
			);
	}

	Vec3 Vec3::operator-(float b) const
	{
		return Vec3(
			values_[0] - b,
			values_[1] - b,
			values_[2] - b
			);
	}

	Vec3 Vec3::operator*(float b) const
	{
		return Vec3(
			values_[0] * b,
			values_[1] * b,
			values_[2] * b
			);
	}

	Vec3 Vec3::operator/(float b) const
	{
		return Vec3(
			values_[0] / b,
			values_[1] / b,
			values_[2] / b
			);
	}


	//
	// op= operators with floats
	//

	Vec3& Vec3::operator+=(float b)
	{
		values_[0] += b;
		values_[1] += b;
		values_[2] += b;

		return *this;
	}

	Vec3& Vec3::operator-=(float b)
	{
		values_[0] -= b;
		values_[1] -= b;
		values_[2] -= b;

		return *this;
	}

	Vec3& Vec3::operator*=(float b)
	{
		values_[0] *= b;
		values_[1] *= b;
		values_[2] *= b;

		return *this;
	}

	Vec3& Vec3::operator/=(float b)
	{
		values_[0] /= b;
		values_[1] /= b;
		values_[2] /= b;

		return *this;
	}


	// negation operator
	Vec3 Vec3::operator-() const
	{
		return Vec3(
			-values_[0],
			-values_[1],
			-values_[2]
			);
	}


	// cross product
	Vec3 Vec3::Cross(const Vec3& b) const
	{
		return Vec3(
			values_[1] * b.values_[2] - values_[2] * b.values_[1],
			values_[2] * b.values_[0] - values_[0] * b.values_[2],
			values_[0] * b.values_[1] - values_[1] * b.values_[0]
			);
	}

	// dot product
	float Vec3::Dot(const Vec3& b) const
	{
		return values_[0] * b.values_[0] + values_[1] * b.values_[1] + values_[2] * b.values_[2];
	}

	// reset vector elements
	void Vec3::Reset()
	{
		std::fill(std::begin(values_), std::end(values_), 0.0f);
	}

	// vector length
	float Vec3::Length() const
	{
		return std::sqrt(Dot(*this));
	}

	// normalize vector length
	Vec3 Vec3::Normalize() const
	{
		return *this / Length();
	}

	// rotate from one vector to another
	Vec3 Vec3::RotateFromTo(const Vec3& fromDirection, const Vec3& toDirection) const
	{
		// cos(theta)
		float cosTheta = fromDirection.Dot(toDirection);
		
		// do not rotate if angle is too small
		if (std::abs(cosTheta - 1.0f) < Util::Eps)
		{
			return *this;
		}

		// flip vector if angle is PI
		if (std::abs(cosTheta + 1.0f) < Util::Eps)
		{
			return -(*this);
		}

		// axis of rotation
		Vec3 rotAxis = fromDirection.Cross(toDirection).Normalize();

		// rotate about axis
		return (*this).RotateAboutAxis(rotAxis, cosTheta);
	}

	Vec3 Vec3::RotateFromTo(const Vec3& fromDirection, const Vec3& toDirection, const Vec3& rotationAxis) const
	{
		// cos(theta)
		float cosTheta = fromDirection.Dot(toDirection);

		// do not rotate if angle is too small
		if (std::abs(cosTheta - 1.0f) < Util::Eps)
		{
			return *this;
		}

		// rotate about axis
		return (*this).RotateAboutAxis(rotationAxis, cosTheta);
	}

	Vec3 Vec3::RotateAboutAxis(const Vec3& rotationAxis, float cosTheta) const
	{
		const float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
		const float a = Dot(rotationAxis) *	(1.0f - cosTheta);
		return rotationAxis * a + (*this) * cosTheta + rotationAxis.Cross(*this) * sinTheta;

		//return Vec3(
		//	n[0] * a + v[0] * cosTheta + (n[1] * v[2] - n[2] * v[1]) * sinTheta,
		//	n[1] * a + v[1] * cosTheta + (n[2] * v[0] - n[0] * v[2]) * sinTheta,
		//	n[2] * a + v[2] * cosTheta + (n[0] * v[1] - n[1] * v[0]) * sinTheta
		//	);
	}


	//
	// arythmetic operators with floats when floats come first
	//

	Vec3 operator+(float a, const Vec3& b)
	{
		return b + a;
	}

	Vec3 operator-(float a, const Vec3& b)
	{
		return Vec3(
			a - b.values_[0],
			a - b.values_[1],
			a - b.values_[2]
			);
	}

	Vec3 operator*(float a, const Vec3& b)
	{
		return b * a;
	}

	Vec3 operator/(float a, const Vec3& b)
	{
		return Vec3(
			a / b.values_[0],
			a / b.values_[1],
			a / b.values_[2]
			);
	}
}
