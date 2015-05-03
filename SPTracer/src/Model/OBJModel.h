#ifndef SPT_OBJ_MODEL_H
#define SPT_OBJ_MODEL_H

#include <string>
#include "Model.h"

namespace SPTracer
{

	class OBJModel : public Model
	{
	public:
		OBJModel(std::string filename);
	};

}

#endif
