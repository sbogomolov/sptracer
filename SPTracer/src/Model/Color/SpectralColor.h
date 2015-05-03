#ifndef SPT_SPECTRAL_COLOR_H
#define SPT_SPECTRAL_COLOR_H

#include <mutex>
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

		void AddAmplitude(float waveLength, float amplitude);
		float GetAmplitude(float waveLength) const override;

	private:
		mutable std::mutex mutex_;
		mutable std::vector<Amplitude> amplitudes_;
		mutable bool initialized_ = false;
	};

}

#endif
