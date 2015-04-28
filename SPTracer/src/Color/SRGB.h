#ifndef SPT_SRGB_H
#define SPT_SRGB_H

#include "RGBConverter.h"

namespace SPTracer
{

	struct Vec3;

	class SRGB : public RGBConverter
	{
	public:
		virtual Vec3 GetRGB(const Vec3& xyz) const override;

	private:
		double GammaCorrect(double c) const;
	};

}

#endif
