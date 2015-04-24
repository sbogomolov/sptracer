#ifndef TRACE_RAY_TASK_H
#define TRACE_RAY_TASK_H

#include "../Ray.h"
#include "Task.h"

namespace SPTracer
{
	class Model;
	class SPTracer;

	class TraceRayTask : public Task
	{
	public:
		TraceRayTask(Ray ray, double waveLength, SPTracer& tracer, size_t pixel);
		void operator()() override;

	private:
		const Ray ray_;
		const double waveLength_;
		const Model& model_;
		const size_t pixel_;
		SPTracer& tracer_;
	};

}

#endif
