#include "../../stdafx.h"
#include "RGBColor.h"

namespace SPTracer
{
	RGBColor::RGBColor(float r, float g, float b)
		: r_(r), g_(g), b_(b)
	{
	}

	float RGBColor::GetAmplitude(float waveLength) const
	{
		static const float WaveLengthAccuracy = 1e-2f;

		// ultra-violet
		if (waveLength < 400.0f)
		{
			return 0.0f;
		}
		
		// blue
		if (waveLength < 500.0f)
		{
			return b_;
		}
		
		// green
		if (waveLength < 600.0f)
		{
			return g_;
		}

		// red
		if (waveLength < 700.0f)
		{
			return r_;
		}

		// infra-red
		return 0.0f;
	}

}
