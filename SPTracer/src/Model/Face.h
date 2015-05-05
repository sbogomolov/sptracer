#ifndef SPT_FACE_H
#define SPT_FACE_H

#include <vector>
#include "../Vec3.h"
#include "Vertex.h"

namespace SPTracer
{

	struct Face
	{
		std::vector<Vertex> vertices;
		Vec3 normal;
	};

}

#endif
