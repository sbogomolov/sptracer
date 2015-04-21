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
		virtual ~Model() { };

	protected:
		Camera camera_;
		std::unordered_map<std::string, std::shared_ptr<Material>> materials_;
		std::vector<std::unique_ptr<Object>> objects_;

		Model() { };
	};

}

#endif
