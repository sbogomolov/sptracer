#ifndef SPT_INTERSECTION_H
#define SPT_INTERSECTION_H

#include <memory>
#include "Vec3.h"

namespace SPTracer
{
	class Object;

	struct Intersection
	{
		Vec3 point;
		Vec3 normal;
		float distance;
		std::shared_ptr<Object> object;
	};

}

#endif
