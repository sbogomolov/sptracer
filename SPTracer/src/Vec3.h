#ifndef SPT_POINT_H
#define SPT_POINT_H

namespace SPTracer
{

	struct Vec3
	{
		float x;
		float y;
		float z;

		void Normalize();
		float EuclideanNorm() const;
		Vec3 RotateFromTo(const Vec3& fromDirection, const Vec3& toDirection);
		Vec3 RotateAboutAxis(const Vec3& rotationAxis, float theta);

		static Vec3 FromPhiThetaNormal(float phi, float theta, const Vec3& n);
		static Vec3 CrossProduct(const Vec3& lhs, const Vec3& rhs);

		friend Vec3 operator+(const Vec3& lhs, const Vec3& rhs);
		friend Vec3 operator+(const Vec3& lhs, const float& rhs);
		friend Vec3 operator+(const float& lhs, const Vec3& rhs);
		friend void operator+=(Vec3& lhs, const Vec3& rhs);
		friend void operator+=(Vec3& lhs, const float& rhs);

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
	};

}

#endif
