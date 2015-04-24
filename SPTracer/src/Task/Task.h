#ifndef SPT_TASK_H
#define SPT_TASK_H

namespace SPTracer
{

	class Task
	{
	public:
		virtual ~Task() { };
		virtual void operator()() = 0;

	protected:
		Task() { };
	};

}

#endif
