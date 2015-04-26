#ifndef SPT_TASK_H
#define SPT_TASK_H

namespace SPTracer
{

	class SPTracer;

	class Task
	{
	public:
		virtual ~Task() { };
		virtual void Run() = 0;

	protected:
		Task(SPTracer& tracer) : tracer_(tracer) { };

		SPTracer& tracer_;
	};

}

#endif
