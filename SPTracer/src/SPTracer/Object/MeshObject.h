#ifndef SPT_MESH_OBJECT_H
#define SPT_MESH_OBJECT_H

#include "../stdafx.h"
#include "../Vec3.h"
#include "Face.h"
#include "Object.h"

namespace SPTracer
{
	class Material;

	class MeshObject : public Object
	{
	public:
		MeshObject(
			std::string name,
			std::shared_ptr<Material> material,
			std::vector<Face> faces);

		virtual bool Intersect(const Ray& ray, Intersection& intersection) const override;

	private:
		std::vector<Face> faces_;
	};

}

#endif
