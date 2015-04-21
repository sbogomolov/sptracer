#include "PlanarMeshObject.h"

namespace SPTracer
{

	PlanarMeshObject::PlanarMeshObject(
		std::string name,
		std::shared_ptr<Material> material,
		std::vector<Vec3> vertices,
		std::vector<unsigned long> outline,
		std::vector<std::vector<unsigned long>> holes)
		: Object(std::move(name), std::move(material)),
		  vertices_(std::move(vertices)),
		  outline_(std::move(outline)),
		  holes_(std::move(holes))
	{
	}

}
