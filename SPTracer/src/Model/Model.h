#ifndef SPT_MODEL_H
#define SPT_MODEL_H

#include <string>
#include <unordered_map>
#include "Camera.h"
#include "Material/Material.h"

namespace SPTracer
{

	class Model
	{
	public:
		virtual inline ~Model() { };

		inline const Camera& GetCamera() const { return camera_; };

	protected:
		Camera camera_;
		std::unordered_map<std::string, std::shared_ptr<Material>> materials_;

		inline Model() { };
	};

}

#endif
