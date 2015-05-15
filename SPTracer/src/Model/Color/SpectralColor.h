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
			float waveLength;	// length
			float amplitude;	// amplitude
		};

		explicit SpectralColor(std::vector<Amplitude> amplitudes);
		float GetAmplitude(float waveLength) const override;

	private:
		std::vector<Amplitude> amplitudes_;
	};

}

#endif
