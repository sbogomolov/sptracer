#include <algorithm>
#include <numeric>
#include "../Intersection.h"
#include "../Spectrum.h"
#include "../Tracer.h"
#include "../Util.h"
#include "../Model/Camera.h"
#include "../Model/Model.h"
#include "../Color/XYZConverter.h"
#include "TaskScheduler.h"
#include "TraceTask.h"

namespace SPTracer
{

	TraceTask::TraceTask(Tracer& tracer)
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

		// spectrum
		static const Spectrum& spectrum = tracer_.GetSpectrum();

		static thread_local std::vector<float> reflectance(spectrum.count);
		static thread_local std::vector<float> radiance(spectrum.count);
		static thread_local std::vector<float> weight(spectrum.count);
		static thread_local std::vector<Vec3> color(width * height);

		// reset all colors
		std::fill(color.begin(), color.end(), Vec3{});

		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width; j++)
			{
				// number of samples per pixel
				unsigned int samples = spectrum.count;

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

				// spawn new ray
				Ray originalRay;
				originalRay.origin = origin;
				originalRay.direction = direction;

				// generate random wave length in case ray will hit some materials
				// with properties dependent on wave length (e.g. refraction)
				originalRay.waveIndex = Util::RandInt(0, spectrum.count - 1);

				// set current ray to the original ray
				Ray* ray = &originalRay;

				// set bounces to 0
				unsigned int bounces = 0;

				// set weight to 1
				std::fill(weight.begin(), weight.end(), 1.0f);

				// trace ray
				while (true)
				{
					// try to find intersection
					Intersection intersection;
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
							// color
							Vec3& c = color[i * width + j];

							// radiance
							intersection.object->GetRadiance(*ray, intersection, spectrum, radiance);

							if (ray->monochromatic)
							{
								// only one radiance with applied weight and emission probability
								float r = radiance[ray->waveIndex] * weight[ray->waveIndex] / emissionProbability;

								// store radiance devided by the number of wave length in spectrum
								c += r * xyzConverter.GetXYZ(spectrum.values[ray->waveIndex]);
							}
							else
							{
								// full spectrum
								for (size_t t = 0; t < spectrum.count; t++)
								{
									// radiance with applied weight and emission probability
									float r = radiance[t] * weight[t] / emissionProbability;

									// store the mean radiance from all wave length
									c += r * xyzConverter.GetXYZ(spectrum.values[t]) / static_cast<float>(spectrum.count);
								}
							}

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
					Ray reflectedRay;
					intersection.object->GetNewRay(*ray, intersection, spectrum, reflectedRay, reflectance);

					// absorption probability for russian roulette
					float absorptionProbability;

					// check monochromaticity
					if (ray->monochromatic)
					{
						// weight ray with the reflactance and reflection probability
						weight[ray->waveIndex] *= reflectance[ray->waveIndex] / reflectionProbability;
						absorptionProbability = 1.0f - reflectance[ray->waveIndex];
					}
					else
					{
						// weight ray with the reflection probability
						for (size_t t = 0; t < spectrum.count; t++)
						{
							weight[t] *= reflectance[t] / reflectionProbability;
						}

						float meanReflectance = std::accumulate(reflectance.begin(), reflectance.end(), 0.0f) / spectrum.count;
						absorptionProbability = 1.0f - meanReflectance;
					}

					// russian roulette
					if (bounces > minBounces)
					{
						if (Util::RandFloat(0.0f, 1.0f) < absorptionProbability)
						{
							// ray was absorped
							break;
						}

						// ray was not absorped, increase its weight
						if (ray->monochromatic)
						{
							weight[ray->waveIndex] /= (1.0f - absorptionProbability);
						}
						else
						{
							for (size_t t = 0; t < spectrum.count; t++)
							{
								weight[t] /= (1.0f - absorptionProbability);
							}
						}

					}

					// change current ray to reflected ray
					ray = &reflectedRay;

					bounces++;
				}
			}
		}

		// add another task
		tracer_.GetTaskScheduler().AddTask(std::make_unique<TraceTask>(tracer_));
		
		// add samples
		tracer_.AddSamples(color);
	}

}
