#include <algorithm>
#include "Box.h"
#include "Intersection.h"
#include "Ray.h"
#include "Util.h"

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
		float t1 = (min_.x - ray.origin.x) * invDirection.x;
		float t2 = (max_.x - ray.origin.x) * invDirection.x;

		float tmin = std::min(t1, t2);
		float tmax = std::max(t1, t2);

		// Y planes
		t1 = (min_.y - ray.origin.y) * invDirection.y;
		t2 = (max_.y - ray.origin.y) * invDirection.y;

		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));

		// Z planes
		t1 = (min_.z - ray.origin.z) * invDirection.z;
		t2 = (max_.z - ray.origin.z) * invDirection.z;

		tmin = std::max(tmin, std::min(t1, t2));
		tmax = std::min(tmax, std::max(t1, t2));

		return tmax > std::max(tmin, 0.0f);
	}

}
