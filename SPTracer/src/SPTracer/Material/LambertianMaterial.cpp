#include "../stdafx.h"
#include "../Exception.h"
#include "../Log.h"
#include "../Util.h"
#include "../Color/Color.h"
#include "../Color/Spectrum.h"
#include "../Model/Model.h"
#include "../Tracer/Intersection.h"
#include "../Tracer/Ray.h"
#include "LambertianMaterial.h"

namespace SPTracer
{

	LambertianMaterial::LambertianMaterial(std::unique_ptr<Color> diffuseReflectance, const Spectrum& spectrum)
		: diffuseReflectance_(std::move(diffuseReflectance))
	{
		// precompute reflectances for spectrum
		precomputedDiffuseReflectance_.resize(spectrum.count);
		std::transform(spectrum.values.begin(), spectrum.values.end(), precomputedDiffuseReflectance_.begin(),
			std::bind(&Color::GetAmplitude, diffuseReflectance_.get(), std::placeholders::_1));

		// diffuse reflectance probability for full spectrum ray
		diffuseReflectionProbability_ = *std::max_element(precomputedDiffuseReflectance_.begin(), precomputedDiffuseReflectance_.end());
	}

	bool LambertianMaterial::GetNewRaySpecular(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const
	{
		std::string msg = "Lambertian material does not support specular reflections";
		Log::Error(msg);
		throw Exception(msg);
	}

	bool LambertianMaterial::IsEmissive() const
	{
		return false;
	}

	bool LambertianMaterial::IsReflective() const
	{
		return true;
	}

	void LambertianMaterial::GetDiffuseReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const
	{
		// get diffuse reflectance
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

	void LambertianMaterial::GetSpecularReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const
	{
		std::string msg = "Lambertian material does not support specular reflectance";
		Log::Error("Lambertian material does not have radiance");
		throw Exception(msg);
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
