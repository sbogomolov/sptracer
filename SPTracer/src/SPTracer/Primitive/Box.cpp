#include "../stdafx.h"
#include "../Util.h"
#include "../Tracer/Intersection.h"
#include "../Tracer/Ray.h"
#include "Box.h"

namespace SPTracer
{
	Box::Box()
	{
	}

	Box::Box(Vec3 min, Vec3 max)
		: min_(std::move(min)), max_(std::move(max))
	{
	}

	const Vec3& Box::min() const
	{
		return min_;
	}

	const Vec3& Box::max() const
	{
		return max_;
	}

	const float Box::GetSurfaceArea() const
	{
		Vec3 d = max_ - min_;
		return 2.0f * (d[0] * d[1] + d[1] * d[2] + d[2] * d[0]);
	}

	bool Box::Intersect(const Ray& ray, const Vec3& invDirection, Vec3& near, Vec3& far)
	{
		//
		// Kay and Kayjia "slabs" method
		//

		float tnear = std::numeric_limits<float>::min();
		float tfar = std::numeric_limits<float>::max();

		for (size_t i = 0; i < 3; i++)
		{
			// check if ray is parallel to the plane
			if (std::abs(ray.direction[i]) < Util::Eps)
			{
				// check if ray is in between the slabs
				if ((ray.origin[i] < min_[i]) || (ray.origin[i] > max_[i]))
				{
					// ray is outside
					return false;
				}

				// ray is in between the slabs, check other planes
				continue;
			}

			// compute the intersection distances to the planes
			float t1 = (min_[i] - ray.origin[i]) * invDirection[i];
			float t2 = (max_[i] - ray.origin[i]) * invDirection[i];
			
			// make sure that t1 <= t2
			if (t1 > t2)
			{
				std::swap(t1, t2);
			}

			// update tnear and tfar
			tnear = std::max(tnear, t1);
			tfar = std::min(tfar, t2);

			// check that we're still inside the box:
			//   tnear > tfar  means that ray misses the box
			//   tfar < 0.0f   means that the box is behind the ray origin
			if ((tnear > tfar) || (tfar < 0.0f))
			{
				return false;
			}
		}

		// we have intersection, let's compute the intersection points
		near = ray.origin + ray.direction * tnear;
		far = ray.origin + ray.direction * tfar;

		return true;
	}

	bool Box::IsPlanar(unsigned char dimension) const
	{
		return (max_[dimension] - min_[dimension]) < Util::Eps;
	}

}
