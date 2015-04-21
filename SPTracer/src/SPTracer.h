#ifndef SPT_SPTRACER_H
#define SPT_SPTRACER_H

#include <memory>
#include <string>
#include "Model/Model.h"
#include "Task/TaskScheduler.h"

namespace SPTracer
{

	class SPTracer
	{
	public:
		SPTracer(std::string fileName, unsigned short numThreads);
		virtual ~SPTracer();

		const Model& GetModel() const;

	private:
		std::unique_ptr<Model> model_;
		std::unique_ptr<TaskScheduler> taskScheduler_;
	};

}

#endif
