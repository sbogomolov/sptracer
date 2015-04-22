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

	Vec3 Vec3::CrossProduct(const Vec3& lhs, const Vec3& rhs)
	{
		return Vec3{
			(lhs.y * rhs.z) - (lhs.z * rhs.y),
			(lhs.z * rhs.x) - (lhs.x * rhs.z),
			(lhs.x * rhs.y) - (lhs.y * rhs.x)
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
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
	}

	void operator+=(Vec3& lhs, const double& rhs)
	{
		lhs.x += rhs;
		lhs.y += rhs;
		lhs.z += rhs;
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
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
	}

	void operator-=(Vec3& lhs, const double& rhs)
	{
		lhs.x -= rhs;
		lhs.y -= rhs;
		lhs.z -= rhs;
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
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;
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
		lhs.x /= rhs;
		lhs.y /= rhs;
		lhs.z /= rhs;
	}


}
