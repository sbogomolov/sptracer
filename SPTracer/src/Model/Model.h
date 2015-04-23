#ifndef SPT_MODEL_H
#define SPT_MODEL_H

#include <string>
#include <unordered_map>
#include "Camera.h"
#include "Material/Material.h"
#include "Object/Object.h"

namespace SPTracer
{

	class Model
	{
	public:
		virtual ~Model();

		bool GetNewRay(const Ray& ray, Ray& newRay) const;
		static double RandDouble(double min, double max);

	protected:
		Camera camera_;
		std::unordered_map<std::string, std::shared_ptr<Material>> materials_;
		std::vector<std::unique_ptr<Object>> objects_;

		Model();
	};

}

#endif
