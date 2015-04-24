#ifndef SPT_PHONG_LUMINAIRE_MATERIAL_H
#define SPT_PHONG_LUMINAIRE_MATERIAL_H

#include <memory>

#include "Material.h"

namespace SPTracer
{

	struct Intersection;
	struct Ray;
	class Color;

	class PhongLuminaireMaterial : public Material
	{
	public:
		PhongLuminaireMaterial(
			std::unique_ptr<Material> reflectiveMaterial,
			std::unique_ptr<Color> radiantExitance,
			double phongExponent);

		bool IsEmissive() const override;
		bool GetNewRay(const Ray& ray, const Intersection& intersection, double waveLength, Ray& newRay, double& reflectance, double& bdrfPdf) const override;

	private:
		std::unique_ptr<Material> reflectiveMaterial_;
		std::unique_ptr<Color> radiantExitance_;
		double phongExponent_;
	};

}

#endif
