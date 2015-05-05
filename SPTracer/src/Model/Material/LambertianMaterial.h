#ifndef SPT_LAMBERTIAN_MATERIAL_H
#define SPT_LAMBERTIAN_MATERIAL_H

#include <mutex>
#include "Material.h"

namespace SPTracer
{
	class Color;

	class LambertianMaterial : public Material
	{
	public:
		LambertianMaterial(std::unique_ptr<Color> diffuseReflectance, const Spectrum& spectrum);
		
		bool IsEmissive() const override;
		void GetNewRay(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const override;
		void GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const override;
		float GetDiffuseReflectivity(int waveIndex) const override;		// use index -1 for average reflectivity
		float GetSpecularReflectivity(int waveIndex) const override;	// use index -1 for average reflectivity

	private:
		std::unique_ptr<Color> diffuseReflectance_;
		std::vector<float> precomputed_;
	};

}

#endif
