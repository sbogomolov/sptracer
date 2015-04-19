#include "Model/MDLAModel.h"
#include "SPTracer.h"

namespace sptracer {

	SPTracer::SPTracer(std::string fileName)
	{
		// create model from file
		model_ = std::make_unique<MDLAModel>(fileName);
	}

	SPTracer::~SPTracer()
	{
	}

}
