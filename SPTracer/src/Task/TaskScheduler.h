#ifndef SPT_TASK_SCHEDULER_H
#define SPT_TASK_SCHEDULER_H

#include <memory>
#include <queue>
#include "Task.h"

namespace SPTracer
{
	class TaskScheduler
	{
	public:
		TaskScheduler(unsigned short numThreads);

	private:
		std::queue<std::unique_ptr<Task>> tasks_;

		void WorkerThread();
	};

}

#endif
