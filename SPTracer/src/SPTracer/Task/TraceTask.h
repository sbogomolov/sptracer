#ifndef TRACE_TASK_H
#define TRACE_TASK_H

#include "../stdafx.h"
#include "../Tracer/Ray.h"
#include "Task.h"

namespace SPTracer
{
	class XYZConverter;
	class Scene;
	class Tracer;

	class TraceTask : public Task
	{
	public:
		explicit TraceTask(Tracer& tracer);

		virtual void Run() override;
	};

}

#endif
