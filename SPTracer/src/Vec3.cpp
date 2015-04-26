#include <cmath>
#include "Vec3.h"

namespace SPTracer
{
	void Vec3::Normalize()
	{
		*this /= this->EuclideanNorm();
	}

	double Vec3::EuclideanNorm() const
	{
		return std::sqrt((*this) * (*this));
	}

	Vec3 Vec3::RotateFromTo(const Vec3& fromDirection, const Vec3& toDirection)
	{
		static const double Eps = 1e-10;

		// rotation angle
		double theta = std::acos(fromDirection * toDirection);
		
		// do not rotate if angle is too small
		if (std::abs(theta) < Eps)
		{
			return *this;
		}

		// axis of rotation
		Vec3 rotAxis = Vec3::CrossProduct(fromDirection, toDirection);

		// rotate about axis
		return (*this).RotateAboutAxis(rotAxis, theta);
	}

	Vec3 Vec3::RotateAboutAxis(const Vec3& rotationAxis, double theta)
	{
		Vec3& v = *this;
		const Vec3& n = rotationAxis;
		const double cosTheta = std::cos(theta);
		const double sinTheta = std::sin(theta);
		const double a = ((n.x * v.x) + (n.y * v.y) + (n.z * v.z)) * (1.0 - cosTheta);

		return Vec3{
			n.x * a + v.x * cosTheta + (-n.z * v.y + n.y * v.z) * sinTheta,	// x
			n.y * a + v.y * cosTheta + (n.z * v.x - n.x * v.z) * sinTheta,	// y
			n.z * a + v.z * cosTheta + (-n.y * v.x + n.x * v.y) * sinTheta	// z
		};
	}

	Vec3 Vec3::FromPhiThetaNormal(double phi, double theta, const Vec3& n)
	{
		// z axis
		static const Vec3 zAxis{ 0.0, 0.0, 1.0 };

		// get vector coordinates
		double sinTheta = std::sin(theta);
		return Vec3{
			sinTheta * std::cos(phi),	// x
			sinTheta * std::sin(phi),	// y
			std::cos(theta)				// z
		}.RotateFromTo(zAxis, n);
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

	Vec3 operator+(const Vec3& lhs, const double& rhs)
	{
		return Vec3{
			lhs.x + rhs,	// x
			lhs.y + rhs,	// y
			lhs.z + rhs		// z
		};
	}

	Vec3 operator+(const double& lhs, const Vec3& rhs)
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

	void operator+=(Vec3& lhs, const double& rhs)
	{
		lhs.x += rhs;		// x
		lhs.y += rhs;		// y
		lhs.z += rhs;		// z
	}

	Vec3 operator-(const Vec3& lhs, const Vec3& rhs)
	{
		return Vec3 {
			lhs.x - rhs.x,	// x
			lhs.y - rhs.y,	// y
			lhs.z - rhs.z	// z
		};
	}

	Vec3 operator-(const Vec3& lhs, const double& rhs)
	{
		return Vec3{
			lhs.x - rhs,	// x
			lhs.y - rhs,	// y
			lhs.z - rhs		// z
		};
	}

	Vec3 operator-(const double& lhs, const Vec3& rhs)
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

	void operator-=(Vec3& lhs, const double& rhs)
	{
		lhs.x -= rhs;		// x
		lhs.y -= rhs;		// y
		lhs.z -= rhs;		// z
	}

	double operator*(const Vec3& lhs, const Vec3& rhs)
	{
		return (lhs.x * rhs.x) + (lhs.y * rhs.y) + (lhs.z * rhs.z);
	}

	Vec3 operator*(const Vec3& lhs, const double& rhs)
	{
		return Vec3{
			lhs.x * rhs,	// x
			lhs.y * rhs,	// x
			lhs.z * rhs		// z
		};
	}

	Vec3 operator*(const double& lhs, const Vec3& rhs)
	{
		return Vec3{
			lhs * rhs.x,	// x
			lhs * rhs.y,	// x
			lhs * rhs.z		// z
		};
	}

	void operator*=(Vec3& lhs, const double& rhs)
	{
		lhs.x *= rhs;		// x
		lhs.y *= rhs;		// y
		lhs.z *= rhs;		// z
	}

	Vec3 operator/(const Vec3& lhs, const double& rhs)
	{
		return Vec3{
			lhs.x / rhs,	// x
			lhs.y / rhs,	// x
			lhs.z / rhs		// z
		};
	}

	void operator/=(Vec3& lhs, const double& rhs)
	{
		lhs.x /= rhs;		// x
		lhs.y /= rhs;		// y
		lhs.z /= rhs;		// z
	}


}
