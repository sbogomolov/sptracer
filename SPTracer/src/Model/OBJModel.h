#ifndef SPT_OBJ_MODEL_H
#define SPT_OBJ_MODEL_H

#include <string>
#include "Model.h"

namespace SPTracer
{
	struct Spectrum;

	class OBJModel : public Model
	{
	public:
		OBJModel(std::string fileName, const Spectrum& spectrum);
	};

}

#endif
