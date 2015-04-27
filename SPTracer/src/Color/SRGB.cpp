#include <cmath>
#include "../Vec3.h"
#include "SRGB.h"

namespace SPTracer
{

	Vec3 SRGB::GetRGB(const Vec3 & xyz) const
	{
		return Vec3{
			GammaCorrect( 3.2406f * xyz.x - 1.5372f * xyz.y - 0.4986f * xyz.z),		// r
			GammaCorrect(-0.9689f * xyz.x + 1.8758f * xyz.y + 0.0415f * xyz.z),		// g
			GammaCorrect( 0.0557f * xyz.x - 0.2040f * xyz.y + 1.0570f * xyz.z)		// b
		};
	}

	float SRGB::GammaCorrect(float c) const
	{
		return c <= 0.0031308f ? 12.92f * c : 1.055f * std::pow(c, 1.0f / 2.4f) - 0.055f;
	}

}
