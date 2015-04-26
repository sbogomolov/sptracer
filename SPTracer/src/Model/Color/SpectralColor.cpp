#include <algorithm>
#include <sstream>
#include "SpectralColor.h"
#include "../../Exception.h"
#include "../../Log.h"

namespace SPTracer
{

	const double SpectralColor::WaveLengthAccuracy = 1e-4;

	SpectralColor::SpectralColor()
	{
	}

	void SpectralColor::AddAmplitude(double waveLength, double amplitude)
	{
		amplitudes_.push_back(Amplitude{ waveLength, amplitude });
	}

	void SpectralColor::Init()
	{
		if (amplitudes_.size() == 0)
		{
			const char* s = "SpectralColor: Trying to get amplitude, but there are no amplitudes added";
			Log::Error(s);
			throw Exception(s);
		}

		// sort by wave length
		std::sort(amplitudes_.begin(), amplitudes_.end(),
			[](const Amplitude& a, const Amplitude& b) { return a.waveLength < b.waveLength; });

		initialized_ = true;
	}

	double SpectralColor::GetAmplitude(double waveLength) const
	{
		// check that color is initialized
		if (!initialized_)
		{
			const char* s = "SpectralColor: Trying to use not initialized spectral color";
			Log::Error(s);
			throw Exception(s);
		}

		// get upper bound
		auto upper = std::upper_bound(amplitudes_.begin(), amplitudes_.end(), waveLength,
			[](const double& a, const Amplitude& b) { return a < b.waveLength; });
		
		// check that we're not lower than the first wave legth
		if (upper == amplitudes_.begin())
		{
			std::ostringstream oss;
			oss << "SpectralColor: Wave length was out of range: "
				<< waveLength << " < " << amplitudes_[0].waveLength;
			Log::Warning(oss.str());

			return 0.0;
		}

		// get lower bound
		auto lower = upper - 1;
		const Amplitude& lowerAmp = *lower;
		if ((waveLength - lowerAmp.waveLength) < WaveLengthAccuracy)
		{
			return lowerAmp.amplitude;
		}

		// check that we're not higher than the last wave legth
		if (upper == amplitudes_.end())
		{
			std::ostringstream oss;
			oss << "SpectralColor: Wave length was out of range: "
				<< waveLength << " > " << amplitudes_[amplitudes_.size() - 1].waveLength;
			Log::Warning(oss.str());

			return 0.0;
		}

		const Amplitude& upperAmp = *upper;

		// return linear interpolation of amplitude
		return lowerAmp.amplitude
			+ ((waveLength - lowerAmp.waveLength) / (upperAmp.waveLength - lowerAmp.waveLength)
			* (upperAmp.amplitude - lowerAmp.amplitude));
	}

}
