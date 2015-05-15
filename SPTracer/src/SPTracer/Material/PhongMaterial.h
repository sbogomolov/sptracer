#ifndef SPT_PHONG_MATERIAL_H
#define SPT_PHONG_MATERIAL_H

#include "../stdafx.h"
#include "Material.h"

namespace SPTracer
{
	class Color;

	class PhongMaterial : public Material
	{
	public:
		PhongMaterial(std::unique_ptr<Color> diffuseReflectance, std::unique_ptr<Color> specularReflectance, float phongExponent, const Spectrum& spectrum);

		bool IsEmissive() const override;
		bool IsReflective() const override;
		bool GetNewRaySpecular(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const override;
		void GetDiffuseReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const override;
		void GetSpecularReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const override;
		void GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const override;
		float GetDiffuseReflectionProbability(int waveIndex) const override;		// use index -1 for average reflectivity
		float GetSpecularReflectionProbability(int waveIndex) const override;		// use index -1 for average reflectivity

	private:
		std::unique_ptr<Color> diffuseReflectance_;
		std::unique_ptr<Color> specularReflectance_;
		float phongExponent_;
		std::vector<float> precomputedDiffuseReflectance_;
		std::vector<float> precomputedSpecularReflectance_;
		float diffuseReflectionProbability_;
		float specularReflectionProbability_;
	};

}

#endif
