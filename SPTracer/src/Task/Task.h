#ifndef SPT_TASK_H
#define SPT_TASK_H

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
		Task(Tracer& tracer) : tracer_(tracer) { };

		Tracer& tracer_;
	};

}

#endif
