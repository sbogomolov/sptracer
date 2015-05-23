#include "../stdafx.h"
#include "../Util.h"
#include "../Tracer/Intersection.h"
#include "../Tracer/Ray.h"
#include "Triangle.h"

namespace SPTracer
{

	Triangle::Triangle(std::shared_ptr<Material> material)
		: Triangle(std::move(material), Vertex{}, Vertex{}, Vertex{})
	{
	}

	Triangle::Triangle(std::shared_ptr<Material> material, Vertex v1, Vertex v2, Vertex v3)
		: Primitive(std::move(material)), vertices_{ std::move(v1), std::move(v2), std::move(v3) }
	{
		// pre-compute edges for ray-triangle intersection test
		e1_ = vertices_[1].coord - vertices_[0].coord;
		e2_ = vertices_[2].coord - vertices_[0].coord;
	}

	Triangle::~Triangle()
	{
	}

	Vertex& Triangle::operator[](size_t index)
	{
		return vertices_[index];
	}

	const Vertex& Triangle::operator[](size_t index) const
	{
		return vertices_[index];
	}

	const Vec3& Triangle::e1() const
	{
		return e1_;
	}

	const Vec3& Triangle::e2() const
	{
		return e2_;
	}

	void Triangle::ComputeNormals()
	{
		// compute normal vector (using cross product)
		Vec3 normal = e1_.Cross(e2_).Normalize();

		// set normal for vertices
		std::for_each(vertices_.begin(), vertices_.end(), [&normal](Vertex& v) { v.normal = normal; });
	}

	bool Triangle::Intersect(const Ray& ray, Intersection& intersection) const
	{
		//
		// Moller–Trumbore intersection algorithm
		//

		Vec3 p = ray.direction.Cross(e2_);
		float det = e1_.Dot(p);

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

		// get first barycentric coordinate
		Vec3 s = ray.origin - vertices_[0].coord;
		float u = invDet * s.Dot(p);

		// check first barycentric coordinate
		if ((u < 0.0f) || (u > 1.0f))
		{
			return false;
		}

		// get second barycentric coordinate
		Vec3 q = s.Cross(e1_);
		float v = invDet * ray.direction.Dot(q);

		// check second and third barycentric coordinates
		if ((v < 0.0f) || ((u + v) > 1.0f))
		{
			return false;
		}

		// at this stage we can compute t to find out where
		// the intersection point is on the line
		float t = invDet * e2_.Dot(q);

		// check intersection
		if (t < Util::Eps)
		{
			// this means that there is a line intersection
			// but not a ray intersection
			return false;
		}

		// ray intersection point
		Vec3 point = ray.origin + t * ray.direction;

		// normal
		intersection.normal = ((1.0f - u - v) * vertices_[0].normal + u * vertices_[1].normal + v * vertices_[2].normal).Normalize();

		// fill the intersection data
		intersection.point = std::move(point);
		// intersection.normal = n;
		intersection.distance = t;

		return true;
	}

}
