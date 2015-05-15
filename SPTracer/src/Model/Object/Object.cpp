#include "../../stdafx.h"
#include "../../Intersection.h"
#include "../../Ray.h"
#include "../../Spectrum.h"
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
		//
		// Moller–Trumbore intersection algorithm
		//

		Vec3 e1 = v2 - v1;
		Vec3 e2 = v3 - v1;

		Vec3 p = Vec3::CrossProduct(ray.direction, e2);
		float det = e1 * p;

		// check determinant
		if (ray.refracted)
		{
			if (det > -Util::Eps)
			{
				// if det is close to 0 - ray lies in plane of triangle
				// if det is positive - ray comes from outside
				return false;
			}
		}
		else
		{
			if (det < Util::Eps)
			{
				// if det is close to 0 - ray lies in plane of triangle
				// if det is negative - ray comes from inside
				return false;
			}
		}

		// invert determinant
		float invDet = 1.0f / det;

		Vec3 s = ray.origin - v1;
		float u = invDet * (s * p);

		if ((u < 0.0f) || (u > 1.0f))
		{
			return false;
		}

		Vec3 q = Vec3::CrossProduct(s, e1);
		float v = invDet * (ray.direction * q);

		if ((v < 0.0f) || ((u + v) > 1.0f))
		{
			return false;
		}

		// at this stage we can compute t to find out where
		// the intersection point is on the line
		float t = invDet * (e2 * q);

		// check intersection
		if (t < Util::Eps)
		{
			// this means that there is a line intersection
			// but not a ray intersection
			return false;
		}

		// ray intersection point
		Vec3 point = ray.origin + t * ray.direction;
		
		// fill the intersection data
		intersection.point = std::move(point);
		intersection.normal = n;
		intersection.distance = t;

		return true;
	}

	void Object::GetNewRayDiffuse(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const
	{
		// get new ray
		material_->GetNewRayDiffuse(ray, intersection, newRay, reflectance);
	}

	bool Object::GetNewRaySpecular(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const
	{
		// get new ray
		return material_->GetNewRaySpecular(ray, intersection, newRay, reflectance);
	}

	void Object::GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const
	{
		material_->GetRadiance(ray, intersection, radiance);
	}

	bool Object::IsEmissive() const
	{
		return material_->IsEmissive();
	}

	bool Object::IsReflective() const
	{
		return material_->IsReflective();
	}

	float Object::GetDiffuseReflectionProbability(int waveIndex) const
	{
		return material_->GetDiffuseReflectionProbability(waveIndex);
	}

	float Object::GetSpecularReflectionProbability(int waveIndex) const
	{
		return material_->GetSpecularReflectionProbability(waveIndex);
	}

}
