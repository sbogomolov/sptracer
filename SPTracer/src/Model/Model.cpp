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
		bool found = false;
		Intersection current;
		// bool foundEmissive = false;		// used for more complicated condition

		for (const auto& o : objects_)
		{
			bool success = o->Intersect(ray, current);
			
			// no intersection
			if (!success)
			{
				continue;
			}

			/////////////////////////////////////////////////////////////////////////////////////
			// NOTE: More complicated condition for badly positioned light sources.
			// This is more complicated condition. It can be used for models where light sourcse
			// have some surfaces behind them. Normally, there should be some distance between a 
			// surface and light source, in which case a simplified condition can be used
			/////////////////////////////////////////////////////////////////////////////////////

			//// if some intersection was already found
			//if (found)
			//{
			//	// difference between distances
			//	float diff = std::abs(current.distance - intersection.distance);

			//	// distance to new intersection is different from distance to old intersection
			//	if (diff > Util::Eps)
			//	{
			//		// new intersection is not closer that the previous one
			//		if (current.distance > intersection.distance)
			//		{
			//			continue;
			//		}
			//	}
			//	else
			//	{
			//		// same distance to intersection
			//		if (foundEmissive)
			//		{
			//			// previous intersection was with emissive material, so it has higher priority
			//			continue;
			//		}
			//	}
			//}

			// new intersection is not closer
			if (found && (current.distance > intersection.distance))
			{
				continue;
			}

			found = true;
			intersection = current;
			intersection.object = o;
			// foundEmissive = o->IsEmissive();		// used for more complicated condition
		}

		return found;
	}

}
