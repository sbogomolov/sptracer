#include "../stdafx.h"
#include "../Util.h"
#include "../Tracer/Intersection.h"
#include "../Tracer/Ray.h"
#include "Face.h"

namespace SPTracer
{

	Face::Face(std::shared_ptr<Material> material)
		: Face(std::move(material), Vertex{}, Vertex{}, Vertex{})
	{
	}

	Face::Face(std::shared_ptr<Material> material, Vertex v1, Vertex v2, Vertex v3)
		: material_(std::move(material)), vertices_{ std::move(v1), std::move(v2), std::move(v3) }
	{
		// pre-compute edges for ray-triangle intersection test
		e1_ = vertices_[1].coord - vertices_[0].coord;
		e2_ = vertices_[2].coord - vertices_[0].coord;
	}

	Face::~Face()
	{
	}

	Vertex& Face::operator[](size_t index)
	{
		return vertices_[index];
	}

	const Vertex& Face::operator[](size_t index) const
	{
		return vertices_[index];
	}

	const Material& Face::material() const
	{
		return *material_;
	}

	const Vec3& Face::e1() const
	{
		return e1_;
	}

	const Vec3& Face::e2() const
	{
		return e2_;
	}

	void Face::ComputeNormals()
	{
		// compute normal vector (using cross product)
		Vec3 normal = Vec3::CrossProduct(e1_, e2_);

		// normalize normal vector
		normal.Normalize();

		// set normal for vertices
		std::for_each(vertices_.begin(), vertices_.end(), [&normal](Vertex& v) { v.normal = normal; });
	}

	bool Face::Intersect(const Ray& ray, Intersection& intersection) const
	{
		//
		// Moller–Trumbore intersection algorithm
		//

		Vec3 p = Vec3::CrossProduct(ray.direction, e2_);
		float det = e1_ * p;

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
		float u = invDet * (s * p);

		// check first barycentric coordinate
		if ((u < 0.0f) || (u > 1.0f))
		{
			return false;
		}

		// get second barycentric coordinate
		Vec3 q = Vec3::CrossProduct(s, e1_);
		float v = invDet * (ray.direction * q);

		// check second and third barycentric coordinates
		if ((v < 0.0f) || ((u + v) > 1.0f))
		{
			return false;
		}

		// at this stage we can compute t to find out where
		// the intersection point is on the line
		float t = invDet * (e2_ * q);

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
		intersection.normal = (1.0f - u - v) * vertices_[0].normal + u * vertices_[1].normal + v * vertices_[2].normal;

		// fill the intersection data
		intersection.point = std::move(point);
		// intersection.normal = n;
		intersection.distance = t;

		return true;
	}

}
