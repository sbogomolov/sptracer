#ifndef TRACE_RAY_TASK_H
#define TRACE_RAY_TASK_H

#include "../Ray.h"
#include "../Model/Model.h"
#include "Task.h"

namespace SPTracer
{

	class TraceRayTask : public Task
	{
	public:
		TraceRayTask(Ray ray, Model& model);
		void operator()() override;

	private:
		Model& model_;
		Ray ray_;
	};

}

#endif
