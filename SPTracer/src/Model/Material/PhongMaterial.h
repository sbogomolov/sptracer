#ifndef SPT_PHONG_MATERIAL_H
#define SPT_PHONG_MATERIAL_H

#include "Material.h"

namespace SPTracer
{
	class Color;

	class PhongMaterial : public Material
	{
	public:
		PhongMaterial(std::unique_ptr<Color> diffuseReflectance, std::unique_ptr<Color> specularReflectance, float specularExponent, const Spectrum& spectrum);

		bool IsEmissive() const override;
		void GetDiffuseReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const override;
		void GetSpecularReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const override;
		float GetSpecularExponent() const;
		void GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const override;
		float GetDiffuseReflectionProbability(int waveIndex) const override;		// use index -1 for average reflectivity
		float GetSpecularReflectionProbability(int waveIndex) const override;		// use index -1 for average reflectivity

	private:
		std::unique_ptr<Color> diffuseReflectance_;
		std::unique_ptr<Color> specularReflectance_;
		float specularExponent_;
		std::vector<float> precomputedDiffuseReflectance_;
		std::vector<float> precomputedSpecularReflectance_;
		float diffuseReflectionProbability_;
		float specularReflectionProbability_;
	};

}

#endif
