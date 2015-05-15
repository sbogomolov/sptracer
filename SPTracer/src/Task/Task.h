#ifndef SPT_TASK_H
#define SPT_TASK_H

#include "../stdafx.h"

namespace SPTracer
{

	class Tracer;

	class Task
	{
	public:
		virtual ~Task() { };

		// Method that will be called when task is supposed to run.
		virtual void Run() = 0;

	protected:
		Tracer& tracer_;

		explicit Task(Tracer& tracer) : tracer_(tracer) { };
	};

}

#endif
