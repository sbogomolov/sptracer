#include <thread>
#include "../Ray.h"
#include "../SPTracer.h"
#include "TaskScheduler.h"

namespace SPTracer
{

	TaskScheduler::TaskScheduler(SPTracer& tracer, size_t numThreads)
		: tracer_(tracer), numThreads_(numThreads)
	{
		// spawn threads
		for (size_t i = 0; i < numThreads_; i++)
		{
			std::thread t(&TaskScheduler::WorkerThread, this);
			t.detach();
		}
	}

	void TaskScheduler::AddTask(std::unique_ptr<Task> task, bool highPriority)
	{
		// lock
		std::unique_lock<std::mutex> lock(mutex_);

		// add task
		if (highPriority)
		{
			highPriorityTasks_.push(std::move(task));
		}
		else
		{
			tasks_.push(std::move(task));
		}

		// notify if there are sleeping threads
		if (sleepingThreads_ > 0)
		{
			lock.unlock();
			cv_.notify_one();
		}
	}

	std::unique_ptr<Task> TaskScheduler::GetTask()
	{
		// lock
		std::unique_lock<std::mutex> lock(mutex_);

		// check if there are any tasks
		if (tasks_.empty() && highPriorityTasks_.empty())
		{
			// no tasks, wait for one
			sleepingThreads_++;
			cv_.wait(lock, [&] { return !tasks_.empty() || !highPriorityTasks_.empty(); });
			sleepingThreads_--;
		}

		// try to get high priority task
		if (!highPriorityTasks_.empty())
		{
			std::unique_ptr<Task> task = std::move(highPriorityTasks_.front());
			highPriorityTasks_.pop();
			return task;
		}

		// get normal task
		std::unique_ptr<Task> task = std::move(tasks_.front());
		tasks_.pop();
		return task;
	}

	size_t TaskScheduler::GetTasksCount()
	{
		// lock
		std::lock_guard<std::mutex> lock(mutex_);

		// return tasks count
		return tasks_.size();
	}

	void TaskScheduler::WorkerThread()
	{
		while (true)
		{
			// get task (will wait for one if queue is empty)
			std::unique_ptr<Task> task = GetTask();

			// run task
			task->Run();
		}
	}

}
