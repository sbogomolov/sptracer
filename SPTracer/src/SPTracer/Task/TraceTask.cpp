#include "../stdafx.h"
#include "../Util.h"
#include "../Color/Spectrum.h"
#include "../Color/XYZConverter.h"
#include "../Scene/Camera.h"
#include "../Scene/Scene.h"
#include "../Primitive/Primitive.h"
#include "../Tracer/Intersection.h"
#include "../Tracer/Tracer.h"
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
		// model
		static const Scene& model = *tracer_.model_;

		// color converter
		static const XYZConverter& xyzConverter = *tracer_.xyzConverter_;
		
		// camera
		static const Camera& camera = tracer_.camera_;
		static const Vec3& origin = camera.p;
		
		// y-axis
		static const Vec3 yAxis(0.0f, 1.0f, 0.0f);

		// z-axis
		static const Vec3 zAxisReversed(0.0f, 0.0f, -1.0f);

		// width and height
		static const unsigned int width = tracer_.width_;
		static const unsigned int height = tracer_.height_;
		static const float pixelWidth = camera.iw / width;
		static const float pixelHeight = camera.ih / height;
		static const float left = camera.icx - camera.iw / 2.0f;
		static const float top = camera.icy + camera.ih / 2.0f;

		// spectrum
		static const Spectrum& spectrum = tracer_.spectrum_;

		static thread_local std::vector<float> reflectance(spectrum.count);
		static thread_local std::vector<float> radiance(spectrum.count);
		static thread_local std::vector<float> weight(spectrum.count);
		static thread_local std::vector<Vec3> color(width * height);

		// reset all colors
		std::for_each(color.begin(), color.end(), [](Vec3& c) { c.Reset(); });

		for (size_t i = 0; i < height; i++)
		{
			for (size_t j = 0; j < width; j++)
			{
				// sample pixel
				float u = left + (static_cast<float>(j) + Util::RandFloat(0.0f, 1.0f)) * pixelWidth;
				float v = top - (static_cast<float>(i) + Util::RandFloat(0.0f, 1.0f)) * pixelHeight;

				// direction
				Vec3 direction = Vec3(
					u,			// x
					v,			// y
					-camera.f	// z
				).Normalize();

				// rotate direction according ti view direction around up axis
				direction = direction.RotateFromTo(zAxisReversed, camera.n, camera.up);

				// rotate direction according to up direction around view direction
				direction = direction.RotateFromTo(yAxis, camera.up, camera.n);

				// spawn new ray
				Ray ray;
				ray.origin = origin;
				ray.direction = direction;

				// originally ray contains all spectrum
				ray.waveIndex = -1;

				// set weight to 1
				std::fill(weight.begin(), weight.end(), 1.0f);

				// trace ray
				while (true)
				{
					// try to find intersection
					Intersection intersection;
					if (!model.Intersect(ray, intersection))
					{
						// no intersection foubd
						break;
					}

					// material
					const auto& material = intersection.primitive->material();

					// reflected (refracted) ray weight correction
					float reflectionProbability = 1.0f;

					// check if light should be emitted
					if (material.IsEmissive())
					{
						// check if material is reflective
						bool reflective = material.IsReflective();

						// emission probability for emissive material
						float emissionProbability = reflective ? 0.9f : 1.0f;

						if (!reflective || (Util::RandFloat(0.0f, 1.0f) < emissionProbability))
						{
							// color
							Vec3& c = color[i * width + j];

							// radiance
							material.GetRadiance(ray, intersection, radiance);

							if (ray.waveIndex == -1)
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
							else
							{
								// only one radiance with applied weight and emission probability
								float r = radiance[ray.waveIndex] * weight[ray.waveIndex] / emissionProbability;

								// store radiance devided by the number of wave length in spectrum
								c += r * xyzConverter.GetXYZ(spectrum.values[ray.waveIndex]);
							}

							// done with this ray
							break;
						}
						else
						{
							// set reflection probability
							reflectionProbability = 1.0f - emissionProbability;
						}
					}

					// preserve monochromaticity, refracted state and the wave index for the ray
					// origin and direction should be set in the GetNewRay method
					Ray newRay;
					newRay.refracted = ray.refracted;
					newRay.waveIndex = ray.waveIndex;

					float diffuseReflectionProbability = material.GetDiffuseReflectionProbability(ray.waveIndex);
					float specularReflectionProbability = material.GetSpecularReflectionProbability(ray.waveIndex);

					/////////////////////////////////////////////////////////////////////////////////////
					// 
					// TODO: take into account refraction. In case of refraction, refracted should be
					//       set to true and waveIndex should be assigned a random index from the range
					//       0 <= waveIndex < spectrum.count.
					//
					// newRay.refracted = true;
					// newRay.waveIndex = Util::RandInt(0, spectrum.count - 1);
					//
					/////////////////////////////////////////////////////////////////////////////////////

					// decide what happens with the ray next
					float next = Util::RandFloat(0.0f, 1.0f);
					if (next < diffuseReflectionProbability)
					{
						// diffuse reflection
						material.GetNewRayDiffuse(ray, intersection, newRay, reflectance);

						// ray was not absorped, increase its weight by decreasing reflection probability
						reflectionProbability *= diffuseReflectionProbability;
					}
					else if (next < (diffuseReflectionProbability + specularReflectionProbability))
					{
						// specular reflection
						if (!material.GetNewRaySpecular(ray, intersection, newRay, reflectance))
						{
							// specular ray points inside the material,
							// stop tracing this path
							break;
						}
						
						// ray was not absorped, increase its weight by decreasing reflection probability
						reflectionProbability *= specularReflectionProbability;
					}
					else
					{
						// ray absorped
						break;
					}

					// update ray weight
					if (ray.waveIndex == -1)
					{
						for (size_t t = 0; t < spectrum.count; t++)
						{
							weight[t] *= reflectance[t] / reflectionProbability;
						}
					}
					else
					{
						weight[ray.waveIndex] *= reflectance[ray.waveIndex] / reflectionProbability;
					}

					// change current ray to reflected (refracted) ray
					std::swap(ray, newRay);
				}
			}
		}

		// add another task
		tracer_.taskScheduler_->AddTask(std::make_unique<TraceTask>(tracer_));
		
		// add samples
		tracer_.AddSamples(color);
	}

}
