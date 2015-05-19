#ifndef SPT_LAMBERTIAN_MATERIAL_H
#define SPT_LAMBERTIAN_MATERIAL_H

#include "../stdafx.h"
#include "Material.h"

namespace SPTracer
{
	class Color;

	class LambertianMaterial : public Material
	{
	public:
		LambertianMaterial(std::unique_ptr<Color> diffuseReflectance, const Spectrum& spectrum);
		
		virtual bool IsEmissive() const override;
		virtual bool IsReflective() const override;
		virtual bool GetNewRaySpecular(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const override;
		virtual void GetDiffuseReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const override;
		virtual void GetSpecularReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const override;
		virtual void GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const override;
		virtual float GetDiffuseReflectionProbability(int waveIndex) const override;		// use index -1 for average reflectivity
		virtual float GetSpecularReflectionProbability(int waveIndex) const override;		// use index -1 for average reflectivity

	private:
		std::unique_ptr<Color> diffuseReflectance_;
		std::vector<float> precomputedDiffuseReflectance_;
		float diffuseReflectionProbability_;
	};

}

#endif
