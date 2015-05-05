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
		for (size_t i = 0; i < spectrum.count; i++)
		{
			precomputedRadiance_[i] = radiantExitance_->GetAmplitude(spectrum.values[i]);
		}
	}

	bool PhongLuminaireMaterial::IsEmissive() const
	{
		return true;
	}

	void PhongLuminaireMaterial::GetNewRay(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const
	{
		reflectiveMaterial_->GetNewRay(ray, intersection, newRay, reflectance);
	}

	void PhongLuminaireMaterial::GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const
	{
		// angle between ray and normal
		float theta = std::acos((-1 * ray.direction) * intersection.normal);

		// cos distribution with Phong exponent
		float weight = std::pow(std::cos(theta), phongExponent_);

		if (ray.monochromatic)
		{
			// get radiant exitance for wave length scaled according to cosine distribution with Phong exponent
			radiance[ray.waveIndex] = weight * precomputedRadiance_[ray.waveIndex];
		}
		else
		{
			// get radiant exitances for spectrum and scale according to cosine distribution with Phong exponent
			for (size_t i = 0; i < precomputedRadiance_.size(); i++)
			{
				radiance[i] = weight * precomputedRadiance_[i];
			}
		}
	}

	float PhongLuminaireMaterial::GetDiffuseReflectance(int waveIndex) const
	{
		return reflectiveMaterial_->GetDiffuseReflectance(waveIndex);
	}

	float PhongLuminaireMaterial::GetSpecularReflectance(int waveIndex) const
	{
		return reflectiveMaterial_->GetSpecularReflectance(waveIndex);
	}

}
