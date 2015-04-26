#include <chrono>
#include <random>
#include "../Intersection.h"
#include "../Ray.h"
#include "Model.h"

namespace SPTracer
{
	Model::Model()
	{
	}

	Model::~Model()
	{
	}

	const Camera & Model::GetCamera() const
	{
		return camera_;
	}

	bool Model::Intersect(const Ray& ray, Intersection& intersection) const
	{
		bool found = false;
		Intersection current;
		for (const auto& o : objects_)
		{
			bool success = o->Intersect(ray, current);
			if (success && (!found || (current.distance < intersection.distance)))
			{
				found = true;
				intersection = current;
				intersection.object = o;
			}
		}

		if (found && (intersection.object == objects_[3]))
		{
			int a = 1;
		}

		return found;
	}

	double Model::RandDouble(double min, double max)
	{
		static thread_local std::mt19937 generator((unsigned int)std::chrono::high_resolution_clock::now().time_since_epoch().count());
		std::uniform_real_distribution<double> distribution(min, max);
		return distribution(generator);
	}

}
