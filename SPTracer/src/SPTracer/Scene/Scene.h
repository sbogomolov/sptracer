#ifndef SPT_SCENE_H
#define SPT_SCENE_H

#include "../stdafx.h"
#include "../Vec3.h"
#include "../Material/Material.h"

namespace SPTracer
{
	struct Intersection;
	struct Ray;
	class Primitive;

	class Scene
	{
		friend class MDLAModel;
		friend class OBJModel;

	public:
		Scene();
		virtual ~Scene();

		bool Intersect(const Ray& ray, Intersection& intersection) const;

	protected:
		std::unordered_map<std::string, std::shared_ptr<Material>> materials_;
		std::vector<std::shared_ptr<Primitive>> primitives_;
	};

}

#endif
