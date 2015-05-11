#ifndef SPT_FACE_H
#define SPT_FACE_H

#include <memory>
#include <vector>
#include "../Vec3.h"

namespace SPTracer
{

	struct Face
	{
		std::vector<std::shared_ptr<Vec3>> vertices;
		bool hasTextureCoordinates = false;
		std::vector<std::shared_ptr<Vec3>> textureCoordinates;
		bool hasVertexNormals = false;
		std::vector<std::shared_ptr<Vec3>> vertexNormals;
		std::vector<Vec3> normals;
	};

}

#endif
