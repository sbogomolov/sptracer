#ifndef SPT_PIXEL_DATA_H
#define SPT_PIXEL_DATA_H

namespace SPTracer
{

	struct Vec3;

	struct PixelData
	{
		double x;
		double y;
		double z;
		unsigned long samples;

		friend void operator+=(PixelData& lhs, const PixelData& rhs);
		friend void operator+=(PixelData& lhs, const Vec3& rhs);
	};

}

#endif
