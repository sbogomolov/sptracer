#include "../Intersection.h"
#include "../SPTracer.h"
#include "../Model/Model.h"
#include "TraceRayTask.h"

namespace SPTracer
{

	TraceRayTask::TraceRayTask(Ray ray, double waveLength, SPTracer& tracer, size_t pixel)
		: Task(), ray_(std::move(ray)), tracer_(tracer), model_(tracer.GetModel()), waveLength_(waveLength), pixel_(pixel)
	{
	}

	void TraceRayTask::operator()()
	{
		// emission probability for emissive material
		static const double emissionProbability = 0.9;

		Intersection intersection;
		const Ray* ray = &ray_;
		Ray newRay;

		double weight = 1.0;
		double x = 0.0;
		double y = 0.0;
		double z = 0.0;

		while (true)
		{
			// try to find intersection
			if (!model_.Intersect(*ray, intersection))
			{
				// no intersection foubd
				break;
			}

			// reflected (refracted) ray weight correction
			double reflectionProbability = 1.0;

			// check if light should be emitted
			if (intersection.object->IsEmissive())
			{
				if (Model::RandDouble(0.0, 1.0) < emissionProbability)
				{
					// end path with following value of emission weighted
					// with the emission probability
					
					//
					// TODO: get x, y, z from weight and waveLength
					//

					// done
					break;
				}
				else
				{
					// set reflection probability
					reflectionProbability = 1.0 - emissionProbability;
				}
			}

			// get new ray from non-emissive material
			double reflectance;
			double bdrfPdf;
			intersection.object->GetNewRay(*ray, intersection, waveLength_, newRay, reflectance, bdrfPdf);

			// weight with reflectance and BDRF / PDF
			weight *= bdrfPdf * reflectance ;

			// weight ray with the probability of reflaction
			weight /= (1.0 - emissionProbability);

			// russian roulette
			double absorptionProbability = 1.0 - reflectance;
			if (Model::RandDouble(0.0, 1.0) < absorptionProbability)
			{
				// ray was absorped
				break;
			}

			// ray was not absorped, increase its weight
			weight /= (1.0 - absorptionProbability);

			// change current ray to new ray
			ray = &newRay;
		}

		// add new sample
		tracer_.AddSample(pixel_, x, y, z);
	}

}
