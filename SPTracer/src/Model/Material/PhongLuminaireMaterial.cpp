#include "../../Intersection.h"
#include "../../Ray.h"
#include "../Color/Color.h"
#include "PhongLuminaireMaterial.h"

namespace SPTracer
{

	PhongLuminaireMaterial::PhongLuminaireMaterial(
		std::unique_ptr<Material> reflectiveMaterial,
		std::unique_ptr<Color> radiantExitance,
		double phongExponent)
			: reflectiveMaterial_(std::move(reflectiveMaterial)),
			  radiantExitance_(std::move(radiantExitance)),
		      phongExponent_(phongExponent)

	{
	}

	bool PhongLuminaireMaterial::IsEmissive() const
	{
		return true;
	}

	bool PhongLuminaireMaterial::GetNewRay(const Ray& ray, const Intersection& intersection, double waveLength, Ray& newRay, double& reflectance, double& bdrfPdf) const
	{
		return reflectiveMaterial_->GetNewRay(ray, intersection, waveLength, newRay, reflectance, bdrfPdf);
	}

}
