#include "../stdafx.h"
#include "../Util.h"
#include "../Tracer/Intersection.h"
#include "../Tracer/Ray.h"
#include "Box.h"

namespace SPTracer
{
	Box::Box(Vec3 min, Vec3 max)
		: min_(std::move(min)), max_(std::move(max))
	{
	}

	bool Box::Intersect(const Ray& ray, const Vec3& invDirection, Intersection& intersection)
	{
		//
		// Kay and Kayjia "slabs" method, optimized by Tavian Barnes
		// http://tavianator.com/fast-branchless-raybounding-box-intersections-part-2-nans/
		//

		// X planes
		float t1 = (min_[0] - ray.origin[0]) * invDirection[0];
		float t2 = (max_[0] - ray.origin[0]) * invDirection[0];

		float tmin = std::min(t1, t2);
		float tmax = std::max(t1, t2);

		// Y and Z planes
		for (size_t i = 1; i < 3; i++)
		{
			t1 = (min_[i] - ray.origin[i]) * invDirection[i];
			t2 = (max_[i] - ray.origin[i]) * invDirection[i];

			tmin = std::max(tmin, std::min(t1, t2));
			tmax = std::min(tmax, std::max(t1, t2));
		}

		return tmax > std::max(tmin, 0.0f);
	}

}
