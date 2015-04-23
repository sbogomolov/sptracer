#define _USE_MATH_DEFINES

#include <cmath>
#include <math.h>
#include "../Model.h"
#include "LambertianMaterial.h"

namespace SPTracer
{

	LambertianMaterial::LambertianMaterial(std::unique_ptr<Color> diffuseReflactance)
		: diffuseReflactance_(std::move(diffuseReflactance))
	{
	}

	bool LambertianMaterial::IsEmissive() const
	{
		return false;
	}

	bool LambertianMaterial::GetNewRay(const Ray& ray, const Intersection& intersection, Ray& newRay) const
	{
		// new ray origin is intersection point
		newRay.origin = intersection.point;

		// wave length is not changed
		newRay.waveLength = ray.waveLength;

		// generate random ray direction using BDRF as PDF
		double phi = Model::RandDouble(0.0, 2.0 * M_PI);
		double rho = std::acos(std::sqrt(Model::RandDouble(0.0, 1.0)));
		double theta = M_PI_2 - rho;
		newRay.direction = Vec3::FromPhiThetaNormal(phi, theta, intersection.normal);

		// BDRF
		double bdrf = diffuseReflactance_->GetAmplitude(ray.waveLength) / M_PI * std::cos(theta);

		// weight is changed according to BDRF
		newRay.weight = ray.weight * bdrf;

		return false;
	}

}
