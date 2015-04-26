#ifndef TRACE_TASK_H
#define TRACE_TASK_H

#include "../Ray.h"
#include "Task.h"

namespace SPTracer
{
	class XYZConverter;
	class Model;
	class SPTracer;

	class TraceTask : public Task
	{
	public:
		TraceTask(SPTracer& tracer);
		void Run() override;
	};

}

#endif
