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

	bool Object::IntersectWithTriangle(const Ray& ray, const Vec3& n, const Vec3& v1, const Vec3& v2, const Vec3& v3, Intersection& intersection)
	{
		Vec3 e1 = v2 - v1;
		Vec3 e2 = v3 - v1;

		Vec3 p = Vec3::CrossProduct(ray.direction, e2);
		float det = e1 * p;

		if (std::abs(det) < Util::Eps)
		{
			// ray lies in plane of triangle
			return false;
		}

		float invDet = 1.0f / det;

		Vec3 s = ray.origin - v1;
		float u = invDet * (s * p);

		if ((u < 0.0f) || (u > 1.0f))
		{
			return false;
		}

		Vec3 q = Vec3::CrossProduct(s, e1);
		float v = invDet * (ray.direction * q);

		if ((v < 0.0f) || ((u + v) > 1.0))
		{
			return false;
		}

		// at this stage we can compute t to find out where
		// the intersection point is on the line
		float t = invDet * (e2 * q);

		if (t > Util::Eps)
		{
			// ray intersection
			intersection.point = ray.origin + t * ray.direction;
			intersection.normal = n;
			intersection.distance = t;

			return true;
		}
		else
		{
			// this means that there is a line intersection
			// but not a ray intersection
			return false;
		}
	}

	bool Object::GetNewRay(const Ray& ray, const Intersection& intersection, float waveLength, Ray& newRay, WeightFactors& weightFactors) const
	{
		return material_->GetNewRay(ray, intersection, waveLength, newRay, weightFactors);
	}

	float Object::GetRadiance(const Ray& ray, const Intersection& intersection, float waveLength) const
	{
		return material_->GetRadiance(ray, intersection, waveLength);
	}

	bool Object::IsEmissive() const
	{
		return material_->IsEmissive();
	}

}
