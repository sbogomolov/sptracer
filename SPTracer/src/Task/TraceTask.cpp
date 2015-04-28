#include "../Intersection.h"
#include "../SPTracer.h"
#include "../Util.h"
#include "../WeightFactors.h"
#include "../Model/Camera.h"
#include "../Model/Model.h"
#include "../Color/XYZConverter.h"
#include "TaskScheduler.h"
#include "TraceTask.h"

namespace SPTracer
{

	TraceTask::TraceTask(SPTracer& tracer)
		: Task(tracer)
	{
	}

	void TraceTask::Run()
	{
		// emission probability for emissive material
		static const float emissionProbability = 0.9f;

		// minimal number of bounces before russian roulette
		static const int minBounces = 5;
		
		// model
		static const Model& model = tracer_.GetModel();

		// color converter
		static const XYZConverter& xyzConverter = tracer_.GetXYZConverter();
		
		// camera
		static const Camera& camera = model.GetCamera();
		static const Vec3& origin = camera.p;
		
		// y-axis
		static const Vec3 yAxis{ 0.0f, 1.0f, 0.0f };

		// z-axis
		static const Vec3 zAxis{ 0.0f, 0.0f, 1.0f };

		// width and height
		static const unsigned int width = tracer_.GetWidth();
		static const unsigned int height = tracer_.GetHeight();
		static const float pixelWidth = camera.iw / width;
		static const float pixelHeight = camera.ih / height;
		static const float left = camera.icx - camera.iw / 2.0f;
		static const float top = camera.icy + camera.ih / 2.0f;

		// wave lengths
		static const float min = tracer_.GetWaveLengthMin();
		static const float max = tracer_.GetWaveLengthMax();
		static const float step = tracer_.GetWaveLengthStep();
		static const size_t count = tracer_.GetWaveLengthCount();

		Ray newRay;
		Intersection intersection;
		WeightFactors weightFactors;
		std::vector<Vec3> color(width * height);
		
		// ray origin is always COP
		Ray generatedRay;
		generatedRay.origin = origin;

		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width; j++)
			{
				// color
				Vec3& xyzColor = color[i * width + j];

				// sample pixel
				float u = left + (static_cast<float>(j) + Util::RandFloat(0.0f, 1.0f)) * pixelWidth;
				float v = top - (static_cast<float>(i) + Util::RandFloat(0.0f, 1.0f)) * pixelHeight;

				// direction
				Vec3 direction{
					-u,			// x
					v,			// y
					camera.f	// z
				};

				// normalize direction
				direction.Normalize();

				// rotate direction
				direction = direction.RotateFromTo(zAxis, camera.n);

				// rotate according to up direction
				direction = direction.RotateFromTo(yAxis, camera.up);

				// set generated ray direction
				generatedRay.direction = direction;

				// go over all wave lengths
				for (size_t w = 0; w < count; w++)
				{
					float waveLength = min + step * static_cast<float>(w);
					Ray* ray = &generatedRay;
					float weight = 1.0f;
					unsigned int bounces = 0;

					// trace ray
					while (true)
					{
						// try to find intersection
						if (!model.Intersect(*ray, intersection))
						{
							// no intersection foubd
							break;
						}

						// reflected (refracted) ray weight correction
						float reflectionProbability = 1.0f;

						// check if light should be emitted
						if (intersection.object->IsEmissive())
						{
							if (Util::RandFloat(0.0f, 1.0f) < emissionProbability)
							{
								// radiance
								float radiance = intersection.object->GetRadiance(*ray, intersection, waveLength);

								// apply ray weight
								radiance *= weight;

								// weight with emission prpbability
								radiance /= emissionProbability;

								// store radiance
								xyzColor += radiance * xyzConverter.GetXYZ(waveLength) / static_cast<float>(count);

								// done
								break;
							}
							else
							{
								// set reflection probability
								reflectionProbability = 1.0f - emissionProbability;
							}
						}

						// get new ray from non-emissive material
						intersection.object->GetNewRay(*ray, intersection, waveLength, newRay, weightFactors);

						// weight with reflectance and BDRF / PDF
						weight *= weightFactors.bdrfPdf * weightFactors.reflectance;

						// weight ray with the probability of reflaction
						weight /= reflectionProbability;

						// russian roulette
						float absorptionProbability = 1.0f - weightFactors.reflectance;
						if (bounces > minBounces)
						{
							if (Util::RandFloat(0.0f, 1.0f) < absorptionProbability)
							{
								// ray was absorped
								break;
							}

							// ray was not absorped, increase its weight
							weight /= (1.0f - absorptionProbability);
						}

						// change current ray to new ray
						ray = &newRay;

						bounces++;
					}
				}
			}
		}

		// add another task
		tracer_.GetTaskScheduler().AddTask(std::make_unique<TraceTask>(tracer_));
		
		// add samples
		tracer_.AddSamples(color);
	}

}
