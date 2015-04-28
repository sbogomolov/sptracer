#ifndef SPT_CIE1931_H
#define SPT_CIE1931_H

#include <unordered_map>
#include "XYZConverter.h"


namespace SPTracer
{

	struct Vec3;

	class CIE1931 : public XYZConverter
	{
	public:
		Vec3 GetXYZ(float waveLength) const override;
		float GetWaveLengthMin() const override;
		float GetWaveLengthMax() const override;
		float GetWaveLengthStep() const override;

	private:
		static const float waveLengthMin_;
		static const float waveLengthMax_;
		static const float waveLengthStep_;
		static const Vec3 colorMap_[];
	};

}

#endif
