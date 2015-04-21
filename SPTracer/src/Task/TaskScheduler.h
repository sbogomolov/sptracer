#ifndef SPT_TASK_SCHEDULER_H
#define SPT_TASK_SCHEDULER_H

#include <memory>
#include <queue>
#include <thread>
#include <vector>
#include "Task.h"

namespace SPTracer
{

	class TaskScheduler
	{
	public:
		TaskScheduler(unsigned short numThreads);

	private:
		std::queue<Task> tasks_;
		std::vector<std::thread> threads_;

		void WorkerThread();
	};

}

#endif
