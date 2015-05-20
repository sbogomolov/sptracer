#ifndef SPT_VERTEX_H
#define SPT_VERTEX_H

#include "../stdafx.h"
#include "../Vec3.h"

namespace SPTracer
{

	struct Vertex
	{
		Vec3 coord;
		Vec3 normal;
		Vec3 tex;
	};

}

#endif
