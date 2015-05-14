#include <cmath>
#include "Util.h"
#include "Vec3.h"

namespace SPTracer
{
	Vec3::Vec3()
		: Vec3(0.0f, 0.0f, 0.0f)
	{
	}

	Vec3::Vec3(float x, float y, float z)
	{
		values_[0] = x;
		values_[1] = y;
		values_[2] = z;
	}

	void Vec3::Normalize()
	{
		*this /= this->EuclideanNorm();
	}

	float Vec3::EuclideanNorm() const
	{
		return std::sqrt((*this) * (*this));
	}

	Vec3 Vec3::RotateFromTo(const Vec3& fromDirection, const Vec3& toDirection)
	{
		// cos(theta)
		float cosTheta = fromDirection * toDirection;
		
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
		Vec3 rotAxis = Vec3::CrossProduct(fromDirection, toDirection);

		// normalize axis of rotation
		rotAxis.Normalize();

		// rotate about axis
		return (*this).RotateAboutAxis(rotAxis, cosTheta);
	}

	Vec3 Vec3::RotateFromTo(const Vec3& fromDirection, const Vec3& toDirection, const Vec3& rotationAxis)
	{
		// cos(theta)
		float cosTheta = fromDirection * toDirection;

		// do not rotate if angle is too small
		if (std::abs(cosTheta - 1.0f) < Util::Eps)
		{
			return *this;
		}

		// rotate about axis
		return (*this).RotateAboutAxis(rotationAxis, cosTheta);
	}

	Vec3 Vec3::RotateAboutAxis(const Vec3& rotationAxis, float cosTheta)
	{
		Vec3& v = *this;
		const Vec3& n = rotationAxis;
		const float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
		const float a = ((n[0] * v[0]) + (n[1] * v[1]) + (n[2] * v[2])) * (1.0f - cosTheta);

		return Vec3(
			n[0] * a + v[0] * cosTheta + (-n[2] * v[1] + n[1] * v[2]) * sinTheta,	// x
			n[1] * a + v[1] * cosTheta + ( n[2] * v[0] - n[0] * v[2]) * sinTheta,	// y
			n[2] * a + v[2] * cosTheta + (-n[1] * v[0] + n[0] * v[1]) * sinTheta	// z
		);
	}

	Vec3 Vec3::FromPhiTheta(float phi, float cosTheta)
	{
		// get vector coordinates
		const float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
		return Vec3(
			sinTheta * std::cos(phi),	// x
			sinTheta * std::sin(phi),	// y
			cosTheta					// z
		);
	}

	Vec3 Vec3::CrossProduct(const Vec3& lhs, const Vec3& rhs)
	{
		return Vec3(
			(lhs[1] * rhs[2]) - (lhs[2] * rhs[1]),	// x
			(lhs[2] * rhs[0]) - (lhs[0] * rhs[2]),	// y
			(lhs[0] * rhs[1]) - (lhs[1] * rhs[0])	// z
		);
	}

	const float& Vec3::operator[](size_t index) const
	{
		return values_[index];
	}

	float& Vec3::operator[](size_t index)
	{
		return values_[index];
	}

	Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
	{
		return Vec3(
			lhs[0] + rhs[0],	// x
			lhs[1] + rhs[1],	// y
			lhs[2] + rhs[2]		// z
		);
	}

	Vec3 operator+(const Vec3& lhs, const float& rhs)
	{
		return Vec3(
			lhs[0] + rhs,	// x
			lhs[1] + rhs,	// y
			lhs[2] + rhs	// z
		);
	}

	Vec3 operator+(const float& lhs, const Vec3& rhs)
	{
		return Vec3(
			lhs + rhs[0],	// x
			lhs + rhs[1],	// y
			lhs + rhs[2]	// z
		);
	}

	void operator+=(Vec3& lhs, const Vec3& rhs)
	{
		lhs[0] += rhs[0];		// x
		lhs[1] += rhs[1];		// y
		lhs[2] += rhs[2];		// z
	}

	void operator+=(Vec3& lhs, const float& rhs)
	{
		lhs[0] += rhs;		// x
		lhs[1] += rhs;		// y
		lhs[2] += rhs;		// z
	}

	Vec3 operator-(const Vec3& rhs)
	{
		return Vec3(
			-rhs[0],	// x
			-rhs[1],	// y
			-rhs[2]		// z
		);
	}

	Vec3 operator-(const Vec3& lhs, const Vec3& rhs)
	{
		return Vec3(
			lhs[0] - rhs[0],	// x
			lhs[1] - rhs[1],	// y
			lhs[2] - rhs[2]		// z
		);
	}

	Vec3 operator-(const Vec3& lhs, const float& rhs)
	{
		return Vec3(
			lhs[0] - rhs,	// x
			lhs[1] - rhs,	// y
			lhs[2] - rhs	// z
		);
	}

	Vec3 operator-(const float& lhs, const Vec3& rhs)
	{
		return Vec3(
			lhs - rhs[0],	// x
			lhs - rhs[1],	// y
			lhs - rhs[2]	// z
		);
	}

	void operator-=(Vec3& lhs, const Vec3& rhs)
	{
		lhs[0] -= rhs[0];		// x
		lhs[1] -= rhs[1];		// y
		lhs[2] -= rhs[2];		// z
	}

	void operator-=(Vec3& lhs, const float& rhs)
	{
		lhs[0] -= rhs;		// x
		lhs[1] -= rhs;		// y
		lhs[2] -= rhs;		// z
	}

	float operator*(const Vec3& lhs, const Vec3& rhs)
	{
		return (lhs[0] * rhs[0]) + (lhs[1] * rhs[1]) + (lhs[2] * rhs[2]);
	}

	Vec3 operator*(const Vec3& lhs, const float& rhs)
	{
		return Vec3(
			lhs[0] * rhs,	// x
			lhs[1] * rhs,	// x
			lhs[2] * rhs	// z
		);
	}

	Vec3 operator*(const float& lhs, const Vec3& rhs)
	{
		return Vec3(
			lhs * rhs[0],	// x
			lhs * rhs[1],	// x
			lhs * rhs[2]	// z
		);
	}

	void operator*=(Vec3& lhs, const float& rhs)
	{
		lhs[0] *= rhs;		// x
		lhs[1] *= rhs;		// y
		lhs[2] *= rhs;		// z
	}

	Vec3 operator/(const Vec3& lhs, const float& rhs)
	{
		return Vec3(
			lhs[0] / rhs,	// x
			lhs[1] / rhs,	// x
			lhs[2] / rhs	// z
		);
	}

	void operator/=(Vec3& lhs, const float& rhs)
	{
		lhs[0] /= rhs;		// x
		lhs[1] /= rhs;		// y
		lhs[2] /= rhs;		// z
	}

}
