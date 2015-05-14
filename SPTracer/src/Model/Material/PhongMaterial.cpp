#include <algorithm>
#include <cmath>
#include <functional>
#include <numeric>
#include <string>
#include "../../Exception.h"
#include "../../Intersection.h"
#include "../../Log.h"
#include "../../Ray.h"
#include "../../Spectrum.h"
#include "../../Util.h"
#include "../Model.h"
#include "../Color/Color.h"
#include "PhongMaterial.h"

namespace SPTracer
{

	PhongMaterial::PhongMaterial(std::unique_ptr<Color> diffuseReflectance, std::unique_ptr<Color> specularReflectance, float phongExponent, const Spectrum& spectrum)
		: diffuseReflectance_(std::move(diffuseReflectance)), specularReflectance_(std::move(specularReflectance)), phongExponent_(phongExponent)
	{
		// precompute diffuse reflectances for spectrum
		precomputedDiffuseReflectance_.resize(spectrum.count);
		std::transform(spectrum.values.begin(), spectrum.values.end(), precomputedDiffuseReflectance_.begin(),
			std::bind(&Color::GetAmplitude, diffuseReflectance_.get(), std::placeholders::_1));

		// precompute specular reflectances for spectrum
		precomputedSpecularReflectance_.resize(spectrum.count);
		std::transform(spectrum.values.begin(), spectrum.values.end(), precomputedSpecularReflectance_.begin(),
			std::bind(&Color::GetAmplitude, specularReflectance_.get(), std::placeholders::_1));

		// probability of reflection
		float reflectionProbability = 0.0f;
		for (size_t i = 0; i < spectrum.count; i++)
		{
			float p = precomputedDiffuseReflectance_[i] + precomputedSpecularReflectance_[i];
			if (p > reflectionProbability)
			{
				reflectionProbability = p;
			}
		}

		// summ of diffuse reflectances
		float sumDiffuse = std::accumulate(precomputedDiffuseReflectance_.begin(), precomputedDiffuseReflectance_.end(), 0.0f);

		// summ of specular reflectances
		float sumSpecular = std::accumulate(precomputedSpecularReflectance_.begin(), precomputedSpecularReflectance_.end(), 0.0f);

		// diffuse reflectance probability for full spectrum ray
		diffuseReflectionProbability_ = sumDiffuse / (sumDiffuse + sumSpecular) * reflectionProbability;
		
		// specular reflectance probability for full spectrum ray
		specularReflectionProbability_ = reflectionProbability - diffuseReflectionProbability_;
	}

	bool PhongMaterial::IsEmissive() const
	{
		return false;
	}

	bool PhongMaterial::IsReflective() const
	{
		return true;
	}

	bool PhongMaterial::GetNewRaySpecular(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const
	{
		// NOTE: Importance sampling.
		// BDRF is 1/pi * cos(theta), it will be used as PDF
		// to prefer bright directions. Because the bright
		// directions are preferred in the choice of samples,
		// we do not have to weight them again by applying
		// the BDRF as a scaling factor to reflectance.
		// Scaling factor in this case is: BDRF/PDF = 1

		// z axis
		static const Vec3 zAxis{ 0.0f, 0.0f, 1.0f };

		// ideal specular reflection direction
		Vec3 specularDirection = ray.direction - 2 * (ray.direction * intersection.normal) * intersection.normal;

		// ideal specular reflection can be obtained by rotation
		// of incident direction about normal on angle of PI.
		// Vec3 specularDirection = (-ray.direction).RotateAboutAxis(intersection.normal, Util::Pi);

		// generate random ray direction using PDF
		float phi = Util::RandFloat(0.0f, 2.0f * Util::Pi);
		float cosAlpha = std::pow(Util::RandFloat(0.0f, 1.0f), 1.0f / (phongExponent_ + 1.0f));
		newRay.direction = Vec3::FromPhiTheta(phi, cosAlpha).RotateFromTo(zAxis, specularDirection);

		// check if direction points inside the material
		if ((newRay.direction * intersection.normal) < Util::Eps)
		{
			// direction points inside the material,
			// stop tracing this path
			return false;
		}

		// new ray origin is intersection point
		newRay.origin = intersection.point;

		// get specular reflectance
		GetSpecularReflectance(ray, intersection, newRay, reflectance);

		return true;
	}

	void PhongMaterial::GetDiffuseReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const
	{
		// get diffuse reflectance
		if (ray.waveIndex == -1)
		{
			// all spectrum
			std::copy(precomputedDiffuseReflectance_.begin(), precomputedDiffuseReflectance_.end(), reflectance.begin());
		}
		else
		{
			// one reflectance
			reflectance[ray.waveIndex] = precomputedDiffuseReflectance_[ray.waveIndex];
		}
	}

	void PhongMaterial::GetSpecularReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const
	{
		// get specular reflectance
		if (ray.waveIndex == -1)
		{
			// all spectrum
			std::copy(precomputedSpecularReflectance_.begin(), precomputedSpecularReflectance_.end(), reflectance.begin());
		}
		else
		{
			// one reflectance
			reflectance[ray.waveIndex] = precomputedSpecularReflectance_[ray.waveIndex];
		}
	}

	void PhongMaterial::GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const
	{
		std::string msg = "Phong material does not have radiance";
		Log::Error("Phong material does not have radiance");
		throw Exception(msg);
	}

	float PhongMaterial::GetDiffuseReflectionProbability(int waveIndex) const
	{
		return waveIndex == -1 ? diffuseReflectionProbability_ : precomputedDiffuseReflectance_[waveIndex];
	}

	float PhongMaterial::GetSpecularReflectionProbability(int waveIndex) const
	{
		return waveIndex == -1 ? specularReflectionProbability_ : precomputedSpecularReflectance_[waveIndex];
	}

}
