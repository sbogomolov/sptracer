#ifndef SPT_UTIL_H
#define SPT_UTIL_H

#include "stdafx.h"

namespace SPTracer
{

	class Util
	{
	public:
		Util() = delete;

		static const float Eps;
		static const float Pi;

		static int RandInt(int min, int max);
		static float RandFloat(float min, float max);
	};

}

#endif
