#ifndef SPT_FACE_H
#define SPT_FACE_H

#include <memory>
#include <vector>
#include "../Vec3.h"
#include "Vertex.h"

namespace SPTracer
{

	struct Face
	{
		std::vector<std::shared_ptr<Vertex>> vertices;
		Vec3 normal;
	};

}

#endif
