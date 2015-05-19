#ifndef SPT_CIE1931_H
#define SPT_CIE1931_H

#include "../stdafx.h"
#include "XYZConverter.h"

namespace SPTracer
{

	class Vec3;

	class CIE1931 : public XYZConverter
	{
	public:
		virtual Vec3 GetXYZ(float waveLength) const override;
		virtual float GetWaveLengthMin() const override;
		virtual float GetWaveLengthMax() const override;
		virtual float GetWaveLengthStep() const override;

	private:
		static const float waveLengthMin_;
		static const float waveLengthMax_;
		static const float waveLengthStep_;
		static const std::vector<Vec3> colorMap_;
	};

}

#endif
