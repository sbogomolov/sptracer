#include "TaskScheduler.h"

#include <thread>

namespace SPTracer
{

	TaskScheduler::TaskScheduler(unsigned short numThreads)
	{
		for (unsigned short i = 0; i < numThreads; i++)
		{
			threads_.push_back(std::thread(&TaskScheduler::WorkerThread, this));
		}
	}

	void TaskScheduler::WorkerThread()
	{
		while (true)
		{
		}
	}

}
