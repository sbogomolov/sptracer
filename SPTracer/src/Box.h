#ifndef SPT_BOX_H
#define SPT_BOX_H

#include "stdafx.h"
#include "Vec3.h"

namespace SPTracer
{

	struct Intersection;
	struct Ray;

	class Box
	{
	public:
		Box(Vec3 min, Vec3 max);
		
		bool Intersect(const Ray& ray, const Vec3& invDirection, Intersection& intersection);

	private:
		Vec3 min_;	// lower limit for coordinates
		Vec3 max_;	// upper limit for coordinates
	};

}

#endif
