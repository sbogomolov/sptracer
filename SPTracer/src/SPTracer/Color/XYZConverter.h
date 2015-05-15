#ifndef SPT_XYZ_CONVERTER_H
#define SPT_XYZ_CONVERTER_H

#include "../stdafx.h"

namespace SPTracer
{

	struct Spectrum;
	class Vec3;

	class XYZConverter
	{
	public:
		virtual ~XYZConverter() { };

		virtual Vec3 GetXYZ(float waveLength) const = 0;
		virtual float GetWaveLengthMin() const = 0;
		virtual float GetWaveLengthMax() const = 0;
		virtual float GetWaveLengthStep() const = 0;

	protected:
		XYZConverter() { };
	};

}

#endif
