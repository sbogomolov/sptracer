#ifndef SPT_OBJ_MATERIAL_H
#define SPT_OBJ_MATERIAL_H

#include "PhongMaterial.h"

namespace SPTracer
{
	class Color;

	class OBJMaterial : public PhongMaterial
	{
	public:
		OBJMaterial(std::unique_ptr<Color> diffuseReflectance, std::unique_ptr<Color> specularReflectance, float specularExponent, const Spectrum& spectrum);

		bool IsEmissive() const override;
		bool IsReflective() const override;
		void GetDiffuseReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const override;
		void GetSpecularReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const override;
		void GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const override;
		float GetSpecularExponent() const override;
		float GetDiffuseReflectionProbability(int waveIndex) const override;		// use index -1 for average reflectivity
		float GetSpecularReflectionProbability(int waveIndex) const override;		// use index -1 for average reflectivity

	private:
	};

}

#endif
