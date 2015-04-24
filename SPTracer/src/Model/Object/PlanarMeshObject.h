#ifndef SPT_PLANAR_MESH_OBJECT_H
#define SPT_PLANAR_MESH_OBJECT_H

#include <memory>
#include <vector>
#include "../../Vec3.h"
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
			std::vector<Vec3> vertices,
			std::vector<unsigned long> outline,
			std::vector<std::vector<unsigned long>> holes);

		bool Intersect(const Ray& ray, Intersection& intersection) const override;

	private:
		Vec3 normal_;
		std::vector<std::vector<unsigned long>> holes_;
		std::vector<unsigned long> outline_;
		std::vector<Vec3> vertices_;
		double d_;
		std::vector<double> dh_;
	};

}

#endif
