#ifndef SPT_RGB_CONVERTER_H
#define SPT_RGB_CONVERTER_H

#include "../stdafx.h"

namespace SPTracer
{

	class Vec3;

	class RGBConverter
	{
	public:
		virtual ~RGBConverter() { };

		virtual Vec3 GetRGB(const Vec3& xyz) const = 0;

	protected:
		RGBConverter() { };
	};

}

#endif
