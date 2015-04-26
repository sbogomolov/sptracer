#ifndef SPT_MODEL_H
#define SPT_MODEL_H

#include <string>
#include <unordered_map>
#include "Camera.h"
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

		const Camera& GetCamera() const;
		bool Intersect(const Ray& ray, Intersection& intersection) const;
		static double RandDouble(double min, double max);

	protected:
		Camera camera_;
		std::unordered_map<std::string, std::shared_ptr<Material>> materials_;
		std::vector<std::shared_ptr<Object>> objects_;

		Model();
	};

}

#endif
