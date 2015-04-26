#define _USE_MATH_DEFINES

#include <cmath>
#include <math.h>
#include "../../Intersection.h"
#include "../../Ray.h"
#include "../../WeightFactors.h"
#include "../Model.h"
#include "../Color/Color.h"
#include "LambertianMaterial.h"

namespace SPTracer
{

	LambertianMaterial::LambertianMaterial(std::unique_ptr<Color> diffuseReflactance)
		: diffuseReflactance_(std::move(diffuseReflactance))
	{
	}

	bool LambertianMaterial::GetNewRay(const Ray& ray, const Intersection& intersection, double waveLength, Ray& newRay, WeightFactors& weightFactors) const
	{
		// NOTE:
		// BDRF is 1/pi * cos(theta), it will be used as PDF
		// to prefer bright directions.

		// new ray origin is intersection point
		newRay.origin = intersection.point;

		// generate random ray direction using BDRF as PDF
		double phi = Model::RandDouble(0.0, 2.0 * M_PI);
		double rho = std::acos(std::sqrt(Model::RandDouble(0.0, 1.0)));
		double theta = M_PI_2 - rho;
		newRay.direction = Vec3::FromPhiThetaNormal(phi, theta, intersection.normal);

		// get reflectance
		weightFactors.reflectance = diffuseReflactance_->GetAmplitude(waveLength);

		// Because the bright directions are preferred in 
		// the choice of samples, we do not have to weight
		// them again by applying the BDRF as a scaling
		// factor to reflectance.
		weightFactors.bdrfPdf = 1.0;

		return false;
	}

	bool LambertianMaterial::IsEmissive() const
	{
		return false;
	}

	double LambertianMaterial::GetLuminance(const Ray & ray, const Intersection & intersection, double waveLength) const
	{
		return 0.0;
	}

}
