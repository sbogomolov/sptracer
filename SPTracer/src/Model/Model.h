#ifndef SPT_MODEL_H
#define SPT_MODEL_H

#include <string>
#include "Camera.h"

namespace sptracer
{

	class Model
	{
	public:
		virtual inline ~Model() = 0 {};

		inline const Camera& camera() const { return camera_; };

	protected:
		Camera camera_;

		inline Model() {};
	};

}

#endif
