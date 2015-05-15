#include "../stdafx.h"
#include "../Exception.h"
#include "../Log.h"
#include "SpectralColor.h"

namespace SPTracer
{

	SpectralColor::SpectralColor(std::vector<Amplitude> amplitudes)
	{
		// check that vector is not empty
		if (amplitudes.size() == 0)
		{
			std::string s = "SpectralColor: Trying to create spectral color with no amplitudes";
			Log::Error(s);
			throw Exception(s);
		}

		// move vector to member variable
		amplitudes_ = std::move(amplitudes);

		// sort by wave length
		std::sort(amplitudes_.begin(), amplitudes_.end(),
			[](const Amplitude& a, const Amplitude& b) { return a.waveLength < b.waveLength; });
	}

	float SpectralColor::GetAmplitude(float waveLength) const
	{
		// wave length accuracy
		static const float WaveLengthAccuracy = 1e-2f;

		// get upper bound
		auto upper = std::upper_bound(amplitudes_.begin(), amplitudes_.end(), waveLength,
			[](const float& a, const Amplitude& b) { return a < b.waveLength; });
		
		// check that we're not lower than the first wave legth
		if (upper == amplitudes_.begin())
		{
			std::ostringstream oss;
			oss << "SpectralColor: Wave length was out of range: "
				<< waveLength << " < " << amplitudes_[0].waveLength;
			Log::Warning(oss.str());

			return 0.0f;
		}

		// get lower bound
		auto lower = upper - 1;
		const Amplitude& lowerAmp = *lower;

		// check if lower bound equals to require wave length with some accuracy
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

			return 0.0f;
		}

		// upper bound amplitude
		const Amplitude& upperAmp = *upper;

		// return linear interpolation of amplitude
		return lowerAmp.amplitude
			+ ((waveLength - lowerAmp.waveLength) / (upperAmp.waveLength - lowerAmp.waveLength)
			* (upperAmp.amplitude - lowerAmp.amplitude));
	}

}
