#ifndef TRACE_TASK_H
#define TRACE_TASK_H

#include "../stdafx.h"
#include "../Ray.h"
#include "Task.h"

namespace SPTracer
{
	class XYZConverter;
	class Model;
	class Tracer;

	class TraceTask : public Task
	{
	public:
		explicit TraceTask(Tracer& tracer);

		void Run() override;
	};

}

#endif
