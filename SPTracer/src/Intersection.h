#ifndef SPT_INTERSECTION_H
#define SPT_INTERSECTION_H

#include "Vec3.h"

namespace SPTracer
{

	struct Intersection
	{
		Vec3 point;
		Vec3 normal;
		double distance;
	};

}

#endif
