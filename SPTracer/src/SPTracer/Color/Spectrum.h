#ifndef SPT_SPECTRUM_H
#define SPT_SPECTRUM_H

#include "../stdafx.h"

namespace SPTracer
{

	struct Spectrum
	{
		float min;
		float max;
		float step;
		unsigned int count;
		std::vector<float> values;
	};

}

#endif
