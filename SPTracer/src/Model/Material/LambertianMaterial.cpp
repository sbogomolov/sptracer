#include <algorithm>
#include <cmath>
#include <functional>
#include <string>
#include "../../Exception.h"
#include "../../Intersection.h"
#include "../../Log.h"
#include "../../Ray.h"
#include "../../Spectrum.h"
#include "../../Util.h"
#include "../Model.h"
#include "../Color/Color.h"
#include "LambertianMaterial.h"

namespace SPTracer
{

	LambertianMaterial::LambertianMaterial(std::unique_ptr<Color> diffuseReflectance, const Spectrum& spectrum)
		: diffuseReflectance_(std::move(diffuseReflectance))
	{
		// precompute reflectances  and absorp probabilities for spectrum
		precomputedDiffuseReflectance_.resize(spectrum.count);
		std::transform(spectrum.values.begin(), spectrum.values.end(), precomputedDiffuseReflectance_.begin(),
			std::bind(&Color::GetAmplitude, diffuseReflectance_.get(), std::placeholders::_1));

		// diffuse reflectance probability for full spectrum ray
		diffuseReflectionProbability_ = *std::max_element(precomputedDiffuseReflectance_.begin(), precomputedDiffuseReflectance_.end());
	}

	void LambertianMaterial::GetNewRayDiffuse(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const
	{
		// NOTE:
		// BDRF is 1/pi * cos(theta), it will be used as PDF
		// to prefer bright directions.

		// new ray origin is intersection point
		newRay.origin = intersection.point;

		// generate random ray direction using BDRF as PDF
		float phi = Util::RandFloat(0.0f, 2.0f * Util::Pi);
		float rho = std::acos(std::sqrt(Util::RandFloat(0.0f, 1.0f)));
		float theta = Util::Pi / 2.0f - rho;
		newRay.direction = Vec3::FromPhiThetaNormal(phi, theta, intersection.normal);

		// NOTE: Importance sampling.
		// Because the bright directions are preferred in 
		// the choice of samples, we do not have to weight
		// them again by applying the BDRF as a scaling
		// factor to reflectance.
		// Scaling factor in this case is: BDRF/PDF = 1

		// get reflectance
		if (ray.waveIndex == -1)
		{
			// all spectrum
			std::copy(precomputedDiffuseReflectance_.begin(), precomputedDiffuseReflectance_.end(), reflectance.begin());
		}
		else
		{
			// one reflectance
			reflectance[ray.waveIndex] = precomputedDiffuseReflectance_[ray.waveIndex];
		}
	}

	void LambertianMaterial::GetNewRaySpecular(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const
	{
		std::string msg = "Lambertian material does not support specular reflections";
		Log::Error(msg);
		throw Exception(msg);
	}

	bool LambertianMaterial::IsEmissive() const
	{
		return false;
	}

	void LambertianMaterial::GetRadiance(const Ray & ray, const Intersection & intersection, std::vector<float>& radiance) const
	{
		std::string msg = "Lambertian material does not have radiance";
		Log::Error("Lambertian material does not have radiance");
		throw Exception(msg);
	}

	float LambertianMaterial::GetDiffuseReflectionProbability(int waveIndex) const
	{
		return waveIndex == -1 ? diffuseReflectionProbability_ : precomputedDiffuseReflectance_[waveIndex];
	}

	float LambertianMaterial::GetSpecularReflectionProbability(int waveIndex) const
	{
		return 0.0f;
	}

}
