#include "PixelData.h"
#include "Vec3.h"

namespace SPTracer
{

	void operator+=(PixelData& lhs, const Vec3& rhs)
	{
		lhs.x += rhs.x;		// x
		lhs.y += rhs.y;		// y
		lhs.z += rhs.z;		// z
	}

}
