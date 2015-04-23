#ifndef SPT_LAMBERTIAN_MATERIAL_H
#define SPT_LAMBERTIAN_MATERIAL_H

#include "Material.h"

namespace SPTracer
{

	class LambertianMaterial : public Material
	{
	public:
		LambertianMaterial(std::unique_ptr<Color> diffuseReflactance);
		
		bool IsEmissive() const override;
		bool GetNewRay(const Ray& ray, const Intersection& intersection, Ray& newRay) const override;

	private:
		std::unique_ptr<Color> diffuseReflactance_;
	};

}

#endif
