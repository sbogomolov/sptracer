#include <cmath>
#include "../../Intersection.h"
#include "../../Ray.h"
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

	bool PhongLuminaireMaterial::GetNewRay(const Ray& ray, const Intersection& intersection, float waveLength, Ray& newRay, WeightFactors& weightFactors) const
	{
		return reflectiveMaterial_->GetNewRay(ray, intersection, waveLength, newRay, weightFactors);
	}

	float PhongLuminaireMaterial::GetRadiance(const Ray& ray, const Intersection& intersection, float waveLength) const
	{
		// get radiant exitance for wave length
		float radiance = radiantExitance_->GetAmplitude(waveLength);

		// angle between ray and normal
		float theta = std::acos((-1 * ray.direction) * intersection.normal);

		// scale according to phong exponent
		radiance *= std::pow(std::cos(theta), phongExponent_);

		return radiance;
	}

}
