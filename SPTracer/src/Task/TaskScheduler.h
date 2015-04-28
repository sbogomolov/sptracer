#ifndef SPT_TASK_SCHEDULER_H
#define SPT_TASK_SCHEDULER_H

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include "Task.h"

namespace SPTracer
{
	class SPTracer;

	class TaskScheduler
	{
	public:
		TaskScheduler(SPTracer& tracer, unsigned int numThreads);

		void AddTask(std::unique_ptr<Task> task, bool highPriority = false);
		std::unique_ptr<Task> GetTask();
		size_t GetTasksCount();

	private:
		SPTracer& tracer_;
		const unsigned int numThreads_;
		unsigned int sleepingThreads_ = 0;
		std::queue<std::unique_ptr<Task>> tasks_;
		std::queue<std::unique_ptr<Task>> highPriorityTasks_;
		std::mutex mutex_;
		std::condition_variable cv_;

		void WorkerThread();
	};

}

#endif
