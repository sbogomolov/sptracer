#ifndef SPT_SCALAR_COLOR_H
#define SPT_SCALAR_COLOR_H

#include "Color.h"

namespace SPTracer
{

	class ScalarColor : public Color
	{
	public:
		ScalarColor(double amplitude);
		double GetAmplitude(double waveLength) const override;

	private:
		double amplitude_;
	};

}

#endif
