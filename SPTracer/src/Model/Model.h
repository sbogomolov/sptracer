#ifndef SPT_MODEL_H
#define SPT_MODEL_H

#include <string>
#include <unordered_map>
#include "../Vec3.h"
#include "Material/Material.h"
#include "Object/Object.h"

namespace SPTracer
{
	struct Intersection;
	struct Ray;

	class Model
	{
	public:
		virtual ~Model();

		bool Intersect(const Ray& ray, Intersection& intersection) const;

	protected:
		std::unordered_map<std::string, std::shared_ptr<Material>> materials_;
		std::vector<std::shared_ptr<Vec3>> vertices_;
		std::vector<std::shared_ptr<Vec3>> vertexNormals_;
		std::vector<std::shared_ptr<Vec3>> textureCoordinates_;
		std::vector<std::shared_ptr<Vec3>> parameterSpaceVertices_;
		std::vector<std::shared_ptr<Object>> objects_;

		Model();
	};

}

#endif
