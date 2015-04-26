#include <cmath>
#include "../../Intersection.h"
#include "../../Ray.h"
#include "../../Util.h"
#include "../Material/Material.h"
#include "Object.h"

namespace SPTracer
{

	Object::Object(std::string name, std::shared_ptr<Material> material)
		: name_(std::move(name)), material_(std::move(material))
	{
	}

	Object::~Object()
	{
	}

	Vec3 Object::ComputeNormal(const Vec3& v1, const Vec3& v2, const Vec3& v3)
	{
		// get two vectors
		Vec3 a = v2 - v1;
		Vec3 b = v3 - v1;

		// compute normal vector (using cross product)
		Vec3 normal = Vec3::CrossProduct(a, b);

		// normalize normal vector
		normal.Normalize();

		return normal;
	}

	bool Object::IntersectWithTriangle(const Ray& ray, const Vec3& n, double d, const Vec3& v1, const Vec3& v2, const Vec3& v3, Intersection& intersection)
	{
		double b = (n.x * ray.direction.x) + (n.y * ray.direction.y) + (n.z * ray.direction.z);
		if (std::abs(b) < Util::Eps)
		{
			// line is parallel to plane
			return false;
		}

		double a = d - (n.x * ray.origin.x) - (n.y * ray.origin.y) - (n.z * ray.origin.z);
		double& t = intersection.distance;
		t = a / b;

		if (t < Util::Eps)
		{
			// ray points in direction oposite from plane or starts from tha plane
			return false;
		}

		// intersection point
		Vec3& p = intersection.point;
		p = ray.origin + ray.direction * t;

		// set normal in intersection point
		intersection.normal = n;

		// compute areas
		double area = Vec3::CrossProduct(v2 - v1, v3 - v1).EuclideanNorm() / 2.0;
		double a1 = Vec3::CrossProduct(v1 - p, v2 - p).EuclideanNorm() / 2.0;
		double a2 = Vec3::CrossProduct(v2 - p, v3 - p).EuclideanNorm() / 2.0;
		double a3 = Vec3::CrossProduct(v3 - p, v1 - p).EuclideanNorm() / 2.0;

		if ((a1 + a2 + a3) > (area + Util::Eps))
		{
			// point is outside the triangle
			return false;
		}

		return true;
	}

	bool Object::GetNewRay(const Ray& ray, const Intersection& intersection, double waveLength, Ray& newRay, WeightFactors& weightFactors) const
	{
		return material_->GetNewRay(ray, intersection, waveLength, newRay, weightFactors);
	}

	double Object::GetRadiance(const Ray& ray, const Intersection& intersection, double waveLength) const
	{
		return material_->GetRadiance(ray, intersection, waveLength);
	}

	bool Object::IsEmissive() const
	{
		return material_->IsEmissive();
	}

}
