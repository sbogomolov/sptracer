#include "Model/MDLAModel.h"
#include "SPTracer.h"

namespace SPTracer {

	SPTracer::SPTracer(std::string fileName)
	{
		// create model from file
		model_ = std::make_unique<MDLAModel>(std::move(fileName));
	}

	SPTracer::~SPTracer()
	{
	}

}
