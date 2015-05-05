#include <cmath>
#include "../../Intersection.h"
#include "../../Ray.h"
#include "../../Spectrum.h"
#include "../../Util.h"
#include "../Model.h"
#include "../Color/Color.h"
#include "LambertianMaterial.h"

namespace SPTracer
{

	LambertianMaterial::LambertianMaterial(std::unique_ptr<Color> diffuseReflectance, const Spectrum& spectrum)
		: diffuseReflectance_(std::move(diffuseReflectance))
	{
		// precompute reflectances for spectrum
		precomputed_.resize(spectrum.count);
		for (size_t i = 0; i < spectrum.count; i++)
		{
			precomputed_[i] = diffuseReflectance_->GetAmplitude(spectrum.values[i]);
		}
	}

	void LambertianMaterial::GetNewRay(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const
	{
		// NOTE:
		// BDRF is 1/pi * cos(theta), it will be used as PDF
		// to prefer bright directions.

		// new ray origin is intersection point
		newRay.origin = intersection.point;

		// generate random ray direction using BDRF as PDF
		float phi = Util::RandFloat(0.0f, 2.0f * Util::Pi);
		float rho = std::acos(std::sqrt(Util::RandFloat(0.0f, 1.0f)));
		float theta = Util::Pi / 2.0f - rho;
		newRay.direction = Vec3::FromPhiThetaNormal(phi, theta, intersection.normal);

		// Because the bright directions are preferred in 
		// the choice of samples, we do not have to weight
		// them again by applying the BDRF as a scaling
		// factor to reflectance.
		static const float bdrfPdf = 1.0f;

		// get reflectance
		if (ray.monochromatic)
		{
			// one reflectance
			reflectance[ray.waveIndex] = bdrfPdf *  precomputed_[ray.waveIndex];
		}
		else
		{
			// all spectrum
			for (size_t i = 0; i < precomputed_.size(); i++)
			{
				reflectance[i] = bdrfPdf * precomputed_[i];
			}
		}
	}

	bool LambertianMaterial::IsEmissive() const
	{
		return false;
	}

	void LambertianMaterial::GetRadiance(const Ray & ray, const Intersection & intersection, std::vector<float>& radiance) const
	{
		// no radiance
	}

	float LambertianMaterial::GetDiffuseReflectivity(int waveIndex) const
	{
		return 0.0f;
	}

	float LambertianMaterial::GetSpecularReflectivity(int waveIndex) const
	{
		return 0.0f;
	}

}
