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
		LambertianMaterial(std::unique_ptr<Color> diffuseReflectance);
		
		void GetNewRay(const Ray& ray, const Intersection& intersection, const Spectrum& spectrum, Ray& newRay, std::vector<float>& reflectance) const override;
		bool IsEmissive() const override;
		void GetRadiance(const Ray& ray, const Intersection& intersection, const Spectrum& spectrum, std::vector<float>& radiance) const override;

	private:
		std::unique_ptr<Color> diffuseReflectance_;
		mutable std::mutex mutex_;
		mutable bool initialized_ = false;
		mutable std::vector<float> precomputed_;
	};

}

#endif
