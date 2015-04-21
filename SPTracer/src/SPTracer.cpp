#include "Model/MDLAModel.h"
#include "SPTracer.h"

namespace SPTracer {

	SPTracer::SPTracer(std::string fileName, unsigned short numThreads)
	{
		// create model from file
		model_ = std::make_unique<MDLAModel>(std::move(fileName));

		// create task scheduler that will spawn threads
		taskScheduler_ = std::make_unique<TaskScheduler>(numThreads);
	}

	SPTracer::~SPTracer()
	{
	}

	const Model& SPTracer::GetModel() const
	{
		return *model_;
	}

}
