#include "TaskScheduler.h"

#include <thread>

namespace SPTracer
{

	TaskScheduler::TaskScheduler(unsigned short numThreads)
	{
		for (unsigned short i = 0; i < numThreads; i++)
		{
			std::thread t(&TaskScheduler::WorkerThread, this);
			t.detach();
		}
	}

	void TaskScheduler::WorkerThread()
	{
		while (true)
		{
		}
	}

}
