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
		Vec3 GetXYZ(double waveLength) const override;
		double GetWaveLengthMin() const override;
		double GetWaveLengthMax() const override;
		double GetWaveLengthStep() const override;

	private:
		static const double waveLengthMin_;
		static const double waveLengthMax_;
		static const double waveLengthStep_;
		static const Vec3 colorMap_[];
	};

}

#endif
