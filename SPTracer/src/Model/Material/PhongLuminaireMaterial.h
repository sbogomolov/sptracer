#ifndef SPT_PHONG_LUMINAIRE_MATERIAL_H
#define SPT_PHONG_LUMINAIRE_MATERIAL_H

#include <memory>
#include "Material.h"

namespace SPTracer
{

	struct Intersection;
	struct Ray;
	struct WeightFactors;
	class Color;

	class PhongLuminaireMaterial : public Material
	{
	public:
		PhongLuminaireMaterial(
			std::unique_ptr<Material> reflectiveMaterial,
			std::unique_ptr<Color> radiantExitance,
			double phongExponent);

		bool GetNewRay(const Ray& ray, const Intersection& intersection, double waveLength, Ray& newRay, WeightFactors& weightFactors) const override;
		bool IsEmissive() const override;
		double GetRadiance(const Ray& ray, const Intersection& intersection, double waveLength) const override;

	private:
		std::unique_ptr<Material> reflectiveMaterial_;
		std::unique_ptr<Color> radiantExitance_;
		double phongExponent_;
	};

}

#endif
