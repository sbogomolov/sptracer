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

		SpectralColor();

		void AddAmplitude(float waveLength, float amplitude);
		void Init();
		float GetAmplitude(float waveLength) const override;

	private:
		static const float WaveLengthAccuracy;
		std::vector<Amplitude> amplitudes_;
		bool initialized_ = false;
	};

}

#endif
