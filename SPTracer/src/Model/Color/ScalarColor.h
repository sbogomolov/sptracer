#ifndef SPT_SCALAR_COLOR_H
#define SPT_SCALAR_COLOR_H

#include "Color.h"

namespace SPTracer
{

	class ScalarColor : public Color
	{
	public:
		explicit ScalarColor(float amplitude);
		float GetAmplitude(float waveLength) const override;

	private:
		float amplitude_;
	};

}

#endif
