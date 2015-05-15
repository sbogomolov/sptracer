#ifndef SPT_FACE_H
#define SPT_FACE_H

#include "../stdafx.h"
#include "../Vec3.h"

namespace SPTracer
{

	struct Face
	{
		std::vector<std::shared_ptr<Vec3>> vertices;
		std::vector<Vec3> normals;
		std::vector<Vec3> e1;
		std::vector<Vec3> e2;

		bool hasTextureCoordinates = false;
		std::vector<std::shared_ptr<Vec3>> textureCoordinates;
		
		bool hasVertexNormals = false;
		std::vector<std::shared_ptr<Vec3>> vertexNormals;
	};

}

#endif
