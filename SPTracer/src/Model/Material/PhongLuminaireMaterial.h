#ifndef SPT_PHONG_LUMINAIRE_MATERIAL_H
#define SPT_PHONG_LUMINAIRE_MATERIAL_H

#include <memory>
#include "../Color/Color.h"
#include "Material.h"

namespace SPTracer
{

	class PhongLuminaireMaterial : public Material
	{
	public:
		PhongLuminaireMaterial(
			std::unique_ptr<Material> reflectiveMaterial,
			std::unique_ptr<Color> radiantExitance,
			double phongExponent);

		bool IsEmissive() const override;

	private:
		std::unique_ptr<Material> reflectiveMaterial_;
		std::unique_ptr<Color> radiantExitance_;
		double phongExponent_;
	};

}

#endif
