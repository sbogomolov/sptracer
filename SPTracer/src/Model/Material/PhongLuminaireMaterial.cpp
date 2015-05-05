#include <algorithm>
#include <cmath>
#include "../../Intersection.h"
#include "../../Ray.h"
#include "../../Spectrum.h"
#include "../Color/Color.h"
#include "PhongLuminaireMaterial.h"

namespace SPTracer
{

	PhongLuminaireMaterial::PhongLuminaireMaterial(
		std::unique_ptr<Material> reflectiveMaterial,
		std::unique_ptr<Color> radiantExitance,
		float phongExponent,
		const Spectrum& spectrum)
			: reflectiveMaterial_(std::move(reflectiveMaterial)),
			  radiantExitance_(std::move(radiantExitance)),
		      phongExponent_(phongExponent)
	{
		// precompute radiances
		precomputedRadiance_.resize(spectrum.count);
		std::transform(spectrum.values.begin(), spectrum.values.end(), precomputedRadiance_.begin(),
			std::bind(&Color::GetAmplitude, radiantExitance_.get(), std::placeholders::_1));
	}

	bool PhongLuminaireMaterial::IsEmissive() const
	{
		return true;
	}

	void PhongLuminaireMaterial::GetNewRayDiffuse(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const
	{
		return reflectiveMaterial_->GetNewRayDiffuse(ray, intersection, newRay, reflectance);
	}

	void PhongLuminaireMaterial::GetNewRaySpecular(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const
	{
		return reflectiveMaterial_->GetNewRaySpecular(ray, intersection, newRay, reflectance);
	}

	void PhongLuminaireMaterial::GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const
	{
		// angle between ray and normal
		float theta = std::acos((-1 * ray.direction) * intersection.normal);

		// cos distribution with Phong exponent
		float weight = std::pow(std::cos(theta), phongExponent_);

		if (ray.waveIndex == -1)
		{
			// get radiant exitances for spectrum and scale according to cosine distribution with Phong exponent
			std::transform(precomputedRadiance_.begin(), precomputedRadiance_.end(), radiance.begin(),
				std::bind(std::multiplies<float>(), weight, std::placeholders::_1));
		}
		else
		{
			// get radiant exitance for wave length scaled according to cosine distribution with Phong exponent
			radiance[ray.waveIndex] = weight * precomputedRadiance_[ray.waveIndex];
		}
	}

	float PhongLuminaireMaterial::GetDiffuseReflectionProbability(int waveIndex) const
	{
		return reflectiveMaterial_->GetDiffuseReflectionProbability(waveIndex);
	}

	float PhongLuminaireMaterial::GetSpecularReflectionProbability(int waveIndex) const
	{
		return reflectiveMaterial_->GetSpecularReflectionProbability(waveIndex);
	}

}
