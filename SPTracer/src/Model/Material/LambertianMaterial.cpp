#include <cmath>
#include "../../Intersection.h"
#include "../../Ray.h"
#include "../../WeightFactors.h"
#include "../../Util.h"
#include "../Model.h"
#include "../Color/Color.h"
#include "LambertianMaterial.h"

namespace SPTracer
{

	LambertianMaterial::LambertianMaterial(std::unique_ptr<Color> diffuseReflactance)
		: diffuseReflactance_(std::move(diffuseReflactance))
	{
	}

	void LambertianMaterial::GetNewRay(const Ray& ray, const Intersection& intersection, double waveLength, Ray& newRay, WeightFactors& weightFactors) const
	{
		// NOTE:
		// BDRF is 1/pi * cos(theta), it will be used as PDF
		// to prefer bright directions.

		// new ray origin is intersection point
		newRay.origin = intersection.point;

		// generate random ray direction using BDRF as PDF
		double phi = Util::RandFloat(0.0, 2.0 * Util::Pi);
		double rho = std::acos(std::sqrt(Util::RandFloat(0.0, 1.0)));
		double theta = Util::Pi / 2.0 - rho;
		newRay.direction = Vec3::FromPhiThetaNormal(phi, theta, intersection.normal);

		// get reflectance
		weightFactors.reflectance = diffuseReflactance_->GetAmplitude(waveLength);

		// Because the bright directions are preferred in 
		// the choice of samples, we do not have to weight
		// them again by applying the BDRF as a scaling
		// factor to reflectance.
		weightFactors.bdrfPdf = 1.0;
	}

	bool LambertianMaterial::IsEmissive() const
	{
		return false;
	}

	double LambertianMaterial::GetRadiance(const Ray & ray, const Intersection & intersection, double waveLength) const
	{
		return 0.0;
	}

}
