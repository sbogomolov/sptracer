#ifndef SPT_POINT_H
#define SPT_POINT_H

namespace SPTracer
{

	struct Vec3
	{
		double x;
		double y;
		double z;

		void Normalize();
		double EuclideanNorm() const;
		Vec3 RotateFromTo(const Vec3& fromDirection, const Vec3& toDirection);
		Vec3 RotateAboutAxis(const Vec3& rotationAxis, double theta);

		static Vec3 FromPhiThetaNormal(double phi, double theta, const Vec3& n);
		static Vec3 CrossProduct(const Vec3& lhs, const Vec3& rhs);

		friend Vec3 operator+(const Vec3& lhs, const Vec3& rhs);
		friend Vec3 operator+(const Vec3& lhs, const double& rhs);
		friend Vec3 operator+(const double& lhs, const Vec3& rhs);
		friend void operator+=(Vec3& lhs, const Vec3& rhs);
		friend void operator+=(Vec3& lhs, const double& rhs);

		friend Vec3 operator-(const Vec3& lhs, const Vec3& rhs);
		friend Vec3 operator-(const Vec3& lhs, const double& rhs);
		friend Vec3 operator-(const double& lhs, const Vec3& rhs);
		friend void operator-=(Vec3& lhs, const Vec3& rhs);
		friend void operator-=(Vec3& lhs, const double& rhs);

		friend double operator*(const Vec3& lhs, const Vec3& rhs);
		friend Vec3 operator*(const Vec3& lhs, const double& rhs);
		friend Vec3 operator*(const double& lhs, const Vec3& rhs);
		friend void operator*=(Vec3& lhs, const double& rhs);

		friend Vec3 operator/(const Vec3& lhs, const double& rhs);
		friend void operator/=(Vec3& lhs, const double& rhs);
	};

}

#endif
