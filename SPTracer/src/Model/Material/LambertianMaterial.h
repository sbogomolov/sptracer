#ifndef SPT_LAMBERTIAN_MATERIAL_H
#define SPT_LAMBERTIAN_MATERIAL_H

#include "Material.h"

namespace SPTracer
{
	struct Intersection;
	struct Ray;
	class Color;

	class LambertianMaterial : public Material
	{
	public:
		LambertianMaterial(std::unique_ptr<Color> diffuseReflactance);
		
		bool IsEmissive() const override;
		bool GetNewRay(const Ray& ray, const Intersection& intersection, double waveLength, Ray& newRay, double& reflectance, double& bdrfPdf) const override;

	private:
		double absorptionProbability_;
		std::unique_ptr<Color> diffuseReflactance_;
	};

}

#endif
