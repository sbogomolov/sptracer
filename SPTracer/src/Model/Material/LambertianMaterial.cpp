#include "LambertianMaterial.h"

namespace SPTracer
{

	LambertianMaterial::LambertianMaterial(std::unique_ptr<Color> diffuseReflactance)
		: diffuseReflactance_(std::move(diffuseReflactance))
	{
	}

	bool LambertianMaterial::IsEmissive() const
	{
		return false;
	}

}
