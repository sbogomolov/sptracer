#include <random>
#include "Model.h"

namespace SPTracer
{
	Model::Model()
	{
	}

	Model::~Model()
	{
	}

	bool Model::GetNewRay(const Ray& ray, Ray& newRay) const
	{
		bool found = false;
		
		Object& object = *objects_[0];
		Intersection best;
		Intersection current;
		for (const auto& o : objects_)
		{
			bool success = o->Intersect(ray, current);
			if (success && (!found || (current.distance < best.distance)))
			{
				found = true;
				best = current;
				object = *o;
			}
		}

		// check if intersection was found
		if (found)
		{
			return object.GetNewRay(ray, best, newRay);
		}

		return false;
	}

	double Model::RandDouble(double min, double max)
	{
		static thread_local std::mt19937 generator;
		std::uniform_real_distribution<double> distribution(min, max);
		return distribution(generator);
	}

}
