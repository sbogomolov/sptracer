#ifndef SPT_LAMBERTIAN_MATERIAL_H
#define SPT_LAMBERTIAN_MATERIAL_H

#include "Material.h"

namespace SPTracer
{
	struct Intersection;
	struct Ray;
	struct WeightFactors;
	class Color;

	class LambertianMaterial : public Material
	{
	public:
		LambertianMaterial(std::unique_ptr<Color> diffuseReflactance);
		
		void GetNewRay(const Ray& ray, const Intersection& intersection, double waveLength, Ray& newRay, WeightFactors& weightFactors) const override;
		bool IsEmissive() const override;
		double GetRadiance(const Ray& ray, const Intersection& intersection, double waveLength) const override;

	private:
		std::unique_ptr<Color> diffuseReflactance_;
	};

}

#endif
