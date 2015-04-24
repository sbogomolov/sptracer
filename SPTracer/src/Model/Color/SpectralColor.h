#ifndef SPT_SPECTRAL_COLOR_H
#define SPT_SPECTRAL_COLOR_H

#include <vector>
#include "Color.h"

namespace SPTracer
{

	class SpectralColor : public Color
	{
	public:
		struct Amplitude
		{
			double waveLength;	// length
			double amplitude;	// amplitude
		};

		SpectralColor();

		void AddAmplitude(double waveLength, double amplitude);
		void Init();
		double GetAmplitude(double waveLength) const override;

	private:
		static const double WaveLengthAccuracy;
		std::vector<Amplitude> amplitudes_;
		bool initialized_ = false;
	};

}

#endif
