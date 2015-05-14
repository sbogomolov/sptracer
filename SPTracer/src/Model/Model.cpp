#include <limits>
#include "../Intersection.h"
#include "../Ray.h"
#include "../Util.h"
#include "Model.h"

namespace SPTracer
{
	Model::Model()
	{
	}

	Model::~Model()
	{
	}

	bool Model::Intersect(const Ray& ray, Intersection& intersection) const
	{
		// set initial intersection distance to max possible,
		// so that any intersection will be closer than that
		intersection.distance = std::numeric_limits<float>::infinity();

		Intersection newIntersection;

		for (const auto& o : objects_)
		{
			// find new intersection
			bool success = o->Intersect(ray, newIntersection);
			
			// check if new intersection is closer
			if (success && (newIntersection.distance < intersection.distance))
			{
				intersection = newIntersection;
				intersection.object = o;
			}
		}

		return intersection.distance < std::numeric_limits<float>::infinity();
	}

}
