#ifndef SPT_PLANAR_MESH_OBJECT_H
#define SPT_PLANAR_MESH_OBJECT_H

#include "../stdafx.h"
#include "../Vec3.h"
#include "Face.h"
#include "Object.h"

namespace SPTracer
{
	class Material;

	class PlanarMeshObject : public Object
	{
	public:
		PlanarMeshObject(
			std::string name,
			std::shared_ptr<Material> material,
			Face face,
			std::vector<Face> holeFaces);

		virtual bool Intersect(const Ray& ray, Intersection& intersection) const override;

	private:
		Face face_;
		std::vector<Face> holeFaces_;
	};

}

#endif
