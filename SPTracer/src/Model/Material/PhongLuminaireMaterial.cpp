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
		double phongExponent)
			: reflectiveMaterial_(std::move(reflectiveMaterial)),
			  radiantExitance_(std::move(radiantExitance)),
		      phongExponent_(phongExponent)

	{
	}

	bool PhongLuminaireMaterial::IsEmissive() const
	{
		return true;
	}

	void PhongLuminaireMaterial::GetNewRay(const Ray& ray, const Intersection& intersection, double waveLength, Ray& newRay, WeightFactors& weightFactors) const
	{
		reflectiveMaterial_->GetNewRay(ray, intersection, waveLength, newRay, weightFactors);
	}

	double PhongLuminaireMaterial::GetRadiance(const Ray& ray, const Intersection& intersection, double waveLength) const
	{
		// get radiant exitance for wave length
		double radiance = radiantExitance_->GetAmplitude(waveLength);

		// angle between ray and normal
		double theta = std::acos((-1 * ray.direction) * intersection.normal);

		// scale according to phong exponent
		radiance *= std::pow(std::cos(theta), phongExponent_);

		return radiance;
	}

}
