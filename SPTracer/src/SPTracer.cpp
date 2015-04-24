#include "Model/MDLAModel.h"
#include "Task/TaskScheduler.h"
#include "SPTracer.h"

namespace SPTracer {

	SPTracer::SPTracer(std::string fileName, unsigned short numThreads, unsigned int width, unsigned int height)
		: width_(width), height_(height)
	{
		// create model from file
		model_ = std::make_unique<MDLAModel>(std::move(fileName));

		// create task scheduler that will spawn threads
		taskScheduler_ = std::make_unique<TaskScheduler>(numThreads);

		// prepare array of pixels
		pixels_.resize((size_t)width_ * height_);
	}

	SPTracer::~SPTracer()
	{
	}

	const Model& SPTracer::GetModel() const
	{
		return *model_;
	}

	void SPTracer::AddSample(size_t pixel, double x, double y, double z)
	{
		// lock
		std::lock_guard<std::mutex> lock(mutex_);
		
		// modify pixel
		PixelData& p = pixels_[pixel];
		p.x += x;
		p.y += y;
		p.z += z;
		p.count++;
	}

}
