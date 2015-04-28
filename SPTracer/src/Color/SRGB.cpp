#include <cmath>
#include "../Vec3.h"
#include "SRGB.h"

namespace SPTracer
{

	Vec3 SRGB::GetRGB(const Vec3& xyz) const
	{
		return Vec3{
			GammaCorrect( 3.2406 * xyz.x - 1.5372 * xyz.y - 0.4986 * xyz.z),	// r
			GammaCorrect(-0.9689 * xyz.x + 1.8758 * xyz.y + 0.0415 * xyz.z),	// g
			GammaCorrect( 0.0557 * xyz.x - 0.2040 * xyz.y + 1.0570 * xyz.z)		// b
		};
	}

	double SRGB::GammaCorrect(double c) const
	{
		return c <= 0.0031308 ? 12.92 * c : 1.055 * std::pow(c, 1.0 / 2.4) - 0.055;
	}

}
