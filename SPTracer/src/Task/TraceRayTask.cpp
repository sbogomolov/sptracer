#include "TraceRayTask.h"

namespace SPTracer
{

	TraceRayTask::TraceRayTask(Ray ray, Model& model)
		: Task(), ray_(std::move(ray)), model_(model)
	{
	}

	void TraceRayTask::operator()()
	{
	}

}
