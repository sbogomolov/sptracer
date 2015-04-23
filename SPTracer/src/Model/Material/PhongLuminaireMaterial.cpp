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

	bool PhongLuminaireMaterial::GetNewRay(const Ray& ray, const Intersection& intersection, Ray& newRay) const
	{
		return reflectiveMaterial_->GetNewRay(ray, intersection, newRay);
	}

}
