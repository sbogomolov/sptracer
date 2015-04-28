#ifndef SPT_PHONG_LUMINAIRE_MATERIAL_H
#define SPT_PHONG_LUMINAIRE_MATERIAL_H

#include <memory>
#include "Material.h"

namespace SPTracer
{

	class Color;

	class PhongLuminaireMaterial : public Material
	{
	public:
		PhongLuminaireMaterial(
			std::unique_ptr<Material> reflectiveMaterial,
			std::unique_ptr<Color> radiantExitance,
			float phongExponent);

		void GetNewRay(const Ray& ray, const Intersection& intersection, const Spectrum& spectrum, Ray& newRay, std::vector<float>& reflectance) const override;
		bool IsEmissive() const override;
		void GetRadiance(const Ray& ray, const Intersection& intersection, const Spectrum& spectrum, std::vector<float>& radiance) const override;

	private:
		std::unique_ptr<Material> reflectiveMaterial_;
		std::unique_ptr<Color> radiantExitance_;
		float phongExponent_;
		mutable bool initialized_;
		mutable std::vector<float> precomputed_;
	};

}

#endif
