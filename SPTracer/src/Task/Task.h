#ifndef SPT_TASK_H
#define SPT_TASK_H

namespace SPTracer
{

	class Task
	{
	public:
		virtual void operator()() = 0;
	};

}

#endif
