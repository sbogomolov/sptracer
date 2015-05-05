#ifndef SPT_VERTEX_H
#define SPT_VERTEX_H

#include "../Vec3.h"

namespace SPTracer
{

	struct Vertex
	{
		Vec3 v;			// geometrical coordinates
		Vec3 vt;		// texture coordinates
		Vec3 vn;		// normal
		Vec3 vp;		// parameter space coordinates
	};

}

#endif
