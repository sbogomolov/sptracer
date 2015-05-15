#ifndef SPT_PIXEL_DATA_H
#define SPT_PIXEL_DATA_H

#include "../stdafx.h"

namespace SPTracer
{

	class Vec3;

	struct PixelData
	{
		double x;
		double y;
		double z;
		unsigned long samples;
	};

}

#endif
