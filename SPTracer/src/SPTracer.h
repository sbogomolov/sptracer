#ifndef SPT_SPTRACER_H
#define SPT_SPTRACER_H

#include <memory>
#include <string>
#include <mutex>
#include <vector>
#include "PixelData.h"

namespace SPTracer
{
	class Model;
	class TaskScheduler;

	class SPTracer
	{
	public:
		SPTracer(std::string fileName, unsigned short numThreads, unsigned int width, unsigned int height);
		virtual ~SPTracer();

		const Model& GetModel() const;
		void AddSample(size_t pixel, double x, double y, double z);

	private:
		std::mutex mutex_;
		unsigned int width_;
		unsigned int height_;
		std::unique_ptr<Model> model_;
		std::unique_ptr<TaskScheduler> taskScheduler_;
		std::vector<PixelData> pixels_;
	};

}

#endif
