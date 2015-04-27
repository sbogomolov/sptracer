#include "ScalarColor.h"

namespace SPTracer
{

	ScalarColor::ScalarColor(float amplitude)
		: amplitude_(amplitude)
	{
	}

	float ScalarColor::GetAmplitude(float waveLength) const
	{
		return amplitude_;
	}

}
