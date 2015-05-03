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
		float phongExponent)
			: reflectiveMaterial_(std::move(reflectiveMaterial)),
			  radiantExitance_(std::move(radiantExitance)),
		      phongExponent_(phongExponent)
	{
	}

	bool PhongLuminaireMaterial::IsEmissive() const
	{
		return true;
	}

	void PhongLuminaireMaterial::GetNewRay(const Ray& ray, const Intersection& intersection, const Spectrum& spectrum, Ray& newRay, std::vector<float>& reflectance) const
	{
		reflectiveMaterial_->GetNewRay(ray, intersection, spectrum, newRay, reflectance);
	}

	void PhongLuminaireMaterial::GetRadiance(const Ray& ray, const Intersection& intersection, const Spectrum& spectrum, std::vector<float>& radiance) const
	{
		// precompute array of radiances for spectrum
		if (!initialized_)
		{
			// lock
			std::lock_guard<std::mutex> lock(mutex_);

			// may be it was initialized in another thread
			if (!initialized_)
			{
				precomputed_.resize(spectrum.count);
				for (size_t i = 0; i < spectrum.count; i++)
				{
					precomputed_[i] = radiantExitance_->GetAmplitude(spectrum.values[i]);
				}

				initialized_ = true;
			}
		}

		// angle between ray and normal
		float theta = std::acos((-1 * ray.direction) * intersection.normal);

		// cos distribution with Phong exponent
		float weight = std::pow(std::cos(theta), phongExponent_);

		if (ray.monochromatic)
		{
			// get radiant exitance for wave length scaled according to cosine distribution with Phong exponent
			radiance[ray.waveIndex] = weight * precomputed_[ray.waveIndex];
		}
		else
		{
			// get radiant exitances for spectrum and scale according to cosine distribution with Phong exponent
			for (size_t i = 0; i < spectrum.count; i++)
			{
				radiance[i] = weight * precomputed_[i];
			}
		}
	}

}
