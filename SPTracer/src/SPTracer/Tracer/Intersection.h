#ifndef SPT_INTERSECTION_H
#define SPT_INTERSECTION_H

#include "../stdafx.h"
#include "../Vec3.h"

namespace SPTracer
{
	class Primitive;

	struct Intersection
	{
		Vec3 point;
		Vec3 normal;
		float distance;
		Primitive* primitive;
	};

}

#endif
