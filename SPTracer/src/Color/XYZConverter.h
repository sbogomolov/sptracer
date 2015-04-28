#ifndef SPT_XYZ_CONVERTER_H
#define SPT_XYZ_CONVERTER_H

namespace SPTracer
{

	struct Vec3;

	class XYZConverter
	{
	public:
		virtual ~XYZConverter() { };

		virtual Vec3 GetXYZ(double waveLength) const = 0;
		virtual double GetWaveLengthMin() const = 0;
		virtual double GetWaveLengthMax() const = 0;
		virtual double GetWaveLengthStep() const = 0;

	protected:
		XYZConverter() { };
	};

}

#endif
