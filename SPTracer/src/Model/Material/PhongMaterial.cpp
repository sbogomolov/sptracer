#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>
#include <string>
#include "../../Exception.h"
#include "../../Intersection.h"
#include "../../Log.h"
#include "../../Ray.h"
#include "../../Spectrum.h"
#include "../../Util.h"
#include "../Model.h"
#include "../Color/Color.h"
#include "PhongMaterial.h"

namespace SPTracer
{

	PhongMaterial::PhongMaterial(std::unique_ptr<Color> diffuseReflectance, std::unique_ptr<Color> specularReflectance, float specularExponent, const Spectrum& spectrum)
		: diffuseReflectance_(std::move(diffuseReflectance)), specularReflectance_(std::move(specularReflectance)), specularExponent_(specularExponent)
	{
		// precompute diffuse reflectances for spectrum
		precomputedDiffuseReflectance_.resize(spectrum.count);
		std::transform(spectrum.values.begin(), spectrum.values.end(), precomputedDiffuseReflectance_.begin(),
			std::bind(&Color::GetAmplitude, diffuseReflectance_.get(), std::placeholders::_1));

		// precompute specular reflectances for spectrum
		precomputedSpecularReflectance_.resize(spectrum.count);
		std::transform(spectrum.values.begin(), spectrum.values.end(), precomputedSpecularReflectance_.begin(),
			std::bind(&Color::GetAmplitude, specularReflectance_.get(), std::placeholders::_1));

		// probability of reflection
		float reflectionProbability = 0.0f;
		for (size_t i = 0; i < spectrum.count; i++)
		{
			float p = precomputedDiffuseReflectance_[i] + precomputedSpecularReflectance_[i];
			if (p > reflectionProbability)
			{
				reflectionProbability = p;
			}
		}

		// summ of diffuse reflectances
		float sumDiffuse = std::accumulate(precomputedDiffuseReflectance_.begin(), precomputedDiffuseReflectance_.end(), 0.0f);

		// summ of specular reflectances
		float sumSpecular = std::accumulate(precomputedSpecularReflectance_.begin(), precomputedSpecularReflectance_.end(), 0.0f);

		// diffuse reflectance probability for full spectrum ray
		diffuseReflectionProbability_ = sumDiffuse / (sumDiffuse + sumSpecular) * reflectionProbability;
		
		// specular reflectance probability for full spectrum ray
		specularReflectionProbability_ = reflectionProbability - diffuseReflectionProbability_;
	}

	bool PhongMaterial::IsEmissive() const
	{
		return false;
	}

	bool PhongMaterial::IsReflective() const
	{
		return false;
	}

	void PhongMaterial::GetDiffuseReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const
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

	void PhongMaterial::GetSpecularReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const
	{
		// get specular reflectance
		if (ray.waveIndex == -1)
		{
			// all spectrum
			std::copy(precomputedSpecularReflectance_.begin(), precomputedSpecularReflectance_.end(), reflectance.begin());
		}
		else
		{
			// one reflectance
			reflectance[ray.waveIndex] = precomputedSpecularReflectance_[ray.waveIndex];
		}
	}

	float PhongMaterial::GetSpecularExponent() const
	{
		return specularExponent_;
	}

	void PhongMaterial::GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const
	{
		std::string msg = "Phong material does not have radiance";
		Log::Error("Phong material does not have radiance");
		throw Exception(msg);
	}

	float PhongMaterial::GetDiffuseReflectionProbability(int waveIndex) const
	{
		return waveIndex == -1 ? diffuseReflectionProbability_ : precomputedDiffuseReflectance_[waveIndex];
	}

	float PhongMaterial::GetSpecularReflectionProbability(int waveIndex) const
	{
		return waveIndex == -1 ? specularReflectionProbability_ : precomputedSpecularReflectance_[waveIndex];
	}

}
