#ifndef SPT_RGB_COLOR_H
#define SPT_RGB_COLOR_H

#include "../stdafx.h"
#include "Color.h"

namespace SPTracer
{

	class RGBColor : public Color
	{
	public:
		RGBColor(float r, float g, float b);

		float GetAmplitude(float waveLength) const override;

	private:
		float r_;
		float g_;
		float b_;
	};

}

#endif
