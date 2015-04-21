#include "ScalarColor.h"

namespace SPTracer
{

	ScalarColor::ScalarColor(double amplitude)
		: amplitude_(amplitude)
	{
	}

	double ScalarColor::GetAmplitude(double waveLength) const
	{
		return amplitude_;
	}

}
