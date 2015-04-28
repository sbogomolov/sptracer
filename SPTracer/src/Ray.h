#ifndef SPT_RAY_H
#define SPT_RAY_H

#include <vector>
#include "Vec3.h"

namespace SPTracer
{
	class Object;

	struct Ray
	{
		Vec3 origin;
		Vec3 direction;
		unsigned int waveIndex;
		bool refracted = false;
		bool monochromatic = false;
	};

}

#endif
