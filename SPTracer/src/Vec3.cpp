#include <cmath>
#include "Util.h"
#include "Vec3.h"

namespace SPTracer
{
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
		const float a = ((n.x * v.x) + (n.y * v.y) + (n.z * v.z)) * (1.0f - cosTheta);

		return Vec3{
			n.x * a + v.x * cosTheta + (-n.z * v.y + n.y * v.z) * sinTheta,	// x
			n.y * a + v.y * cosTheta + ( n.z * v.x - n.x * v.z) * sinTheta,	// y
			n.z * a + v.z * cosTheta + (-n.y * v.x + n.x * v.y) * sinTheta	// z
		};
	}

	Vec3 Vec3::FromPhiTheta(float phi, float cosTheta)
	{
		// get vector coordinates
		const float sinTheta = std::sqrt(1.0f - cosTheta * cosTheta);
		return Vec3{
			sinTheta * std::cos(phi),	// x
			sinTheta * std::sin(phi),	// y
			cosTheta					// z
		};
	}

	Vec3 Vec3::CrossProduct(const Vec3& lhs, const Vec3& rhs)
	{
		return Vec3{
			(lhs.y * rhs.z) - (lhs.z * rhs.y),	// x
			(lhs.z * rhs.x) - (lhs.x * rhs.z),	// y
			(lhs.x * rhs.y) - (lhs.y * rhs.x)	// z
		};
	}

	Vec3 operator+(const Vec3& lhs, const Vec3& rhs)
	{
		return Vec3{
			lhs.x + rhs.x,	// x
			lhs.y + rhs.y,	// y
			lhs.z + rhs.z	// z
		};
	}

	Vec3 operator+(const Vec3& lhs, const float& rhs)
	{
		return Vec3{
			lhs.x + rhs,	// x
			lhs.y + rhs,	// y
			lhs.z + rhs		// z
		};
	}

	Vec3 operator+(const float& lhs, const Vec3& rhs)
	{
		return Vec3{
			lhs + rhs.x,	// x
			lhs + rhs.y,	// y
			lhs + rhs.z		// z
		};
	}

	void operator+=(Vec3& lhs, const Vec3& rhs)
	{
		lhs.x += rhs.x;		// x
		lhs.y += rhs.y;		// y
		lhs.z += rhs.z;		// z
	}

	void operator+=(Vec3& lhs, const float& rhs)
	{
		lhs.x += rhs;		// x
		lhs.y += rhs;		// y
		lhs.z += rhs;		// z
	}

	Vec3 operator-(const Vec3& rhs)
	{
		return Vec3{
			-rhs.x,	// x
			-rhs.y,	// y
			-rhs.z	// z
		};
	}

	Vec3 operator-(const Vec3& lhs, const Vec3& rhs)
	{
		return Vec3 {
			lhs.x - rhs.x,	// x
			lhs.y - rhs.y,	// y
			lhs.z - rhs.z	// z
		};
	}

	Vec3 operator-(const Vec3& lhs, const float& rhs)
	{
		return Vec3{
			lhs.x - rhs,	// x
			lhs.y - rhs,	// y
			lhs.z - rhs		// z
		};
	}

	Vec3 operator-(const float& lhs, const Vec3& rhs)
	{
		return Vec3{
			lhs - rhs.x,	// x
			lhs - rhs.y,	// y
			lhs - rhs.z		// z
		};
	}

	void operator-=(Vec3& lhs, const Vec3& rhs)
	{
		lhs.x -= rhs.x;		// x
		lhs.y -= rhs.y;		// y
		lhs.z -= rhs.z;		// z
	}

	void operator-=(Vec3& lhs, const float& rhs)
	{
		lhs.x -= rhs;		// x
		lhs.y -= rhs;		// y
		lhs.z -= rhs;		// z
	}

	float operator*(const Vec3& lhs, const Vec3& rhs)
	{
		return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
	}

	Vec3 operator*(const Vec3& lhs, const float& rhs)
	{
		return Vec3{
			lhs.x * rhs,	// x
			lhs.y * rhs,	// x
			lhs.z * rhs		// z
		};
	}

	Vec3 operator*(const float& lhs, const Vec3& rhs)
	{
		return Vec3{
			lhs * rhs.x,	// x
			lhs * rhs.y,	// x
			lhs * rhs.z		// z
		};
	}

	void operator*=(Vec3& lhs, const float& rhs)
	{
		lhs.x *= rhs;		// x
		lhs.y *= rhs;		// y
		lhs.z *= rhs;		// z
	}

	Vec3 operator/(const Vec3& lhs, const float& rhs)
	{
		return Vec3{
			lhs.x / rhs,	// x
			lhs.y / rhs,	// x
			lhs.z / rhs		// z
		};
	}

	void operator/=(Vec3& lhs, const float& rhs)
	{
		lhs.x /= rhs;		// x
		lhs.y /= rhs;		// y
		lhs.z /= rhs;		// z
	}


}
