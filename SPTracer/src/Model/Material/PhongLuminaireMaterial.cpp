#include <algorithm>
#include <cmath>
#include <functional>
#include "../../Intersection.h"
#include "../../Ray.h"
#include "../../Spectrum.h"
#include "../../Util.h"
#include "../Color/Color.h"
#include "PhongLuminaireMaterial.h"

namespace SPTracer
{

	PhongLuminaireMaterial::PhongLuminaireMaterial(
		std::unique_ptr<Material> reflectiveMaterial,
		std::unique_ptr<Color> radiance,
		float phongExponent,
		const Spectrum& spectrum)
			: reflectiveMaterial_(std::move(reflectiveMaterial)),
			  radiance_(std::move(radiance)),
		      phongExponent_(phongExponent)
	{
		// precompute radiances
		precomputedRadiance_.resize(spectrum.count);
		std::transform(spectrum.values.begin(), spectrum.values.end(), precomputedRadiance_.begin(),
			std::bind(&Color::GetAmplitude, radiance_.get(), std::placeholders::_1));

		// determine if material is reflective
		reflective_ = reflectiveMaterial_ &&
			((reflectiveMaterial_->GetDiffuseReflectionProbability(-1) + reflectiveMaterial_->GetSpecularReflectionProbability(-1)) > Util::Eps);

		// determine if Phong exponent is used
		phongExponentUsed_ = std::abs(1.0f - phongExponent_) > Util::Eps;
	}

	bool PhongLuminaireMaterial::IsEmissive() const
	{
		return true;
	}

	bool PhongLuminaireMaterial::IsReflective() const
	{
		return reflective_;
	}

	void PhongLuminaireMaterial::GetNewRayDiffuse(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const
	{
		reflectiveMaterial_->GetNewRayDiffuse(ray, intersection, newRay, reflectance);
	}

	bool PhongLuminaireMaterial::GetNewRaySpecular(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const
	{
		return reflectiveMaterial_->GetNewRaySpecular(ray, intersection, newRay, reflectance);
	}

	void PhongLuminaireMaterial::GetDiffuseReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const
	{
		return reflectiveMaterial_->GetDiffuseReflectance(ray, intersection, newRay, reflectance);
	}

	void PhongLuminaireMaterial::GetSpecularReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const
	{
		return reflectiveMaterial_->GetSpecularReflectance(ray, intersection, newRay, reflectance);
	}

	void PhongLuminaireMaterial::GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const
	{
		// angle between ray and normal
		float theta = Util::Pi - std::acos(ray.direction * intersection.normal);

		// cos distribution with Phong exponent
		float weight = phongExponentUsed_
			? std::pow(std::cos(theta), phongExponent_)
			: std::cos(theta);

		if (ray.waveIndex == -1)
		{
			// get radiance for spectrum and scale according to cosine distribution with Phong exponent
			std::transform(precomputedRadiance_.begin(), precomputedRadiance_.end(), radiance.begin(),
				std::bind(std::multiplies<float>(), weight, std::placeholders::_1));
		}
		else
		{
			// get radiance for wave length scaled according to cosine distribution with Phong exponent
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
