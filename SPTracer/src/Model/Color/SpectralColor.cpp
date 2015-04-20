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
		initialized_ = true;
		amplitudes_.push_back(Amplitude{ waveLength, amplitude });
	}

	double SpectralColor::GetAmplitude(double waveLength) const
	{
		// check that color is initialized
		if (!initialized_)
		{
			const char* s = "SpectralColor: Trying to get amplitude, but there are no amplitudes added";
			Log::Error(s);
			throw Exception(s);
		}

		// get upper bound
		auto upper = std::upper_bound(amplitudes_.begin(), amplitudes_.end(), waveLength);
		
		// check that we're not lower than the first wave legth
		if (upper == amplitudes_.begin())
		{
			std::ostringstream oss;
			oss << "SpectralColor: Wave length was out of range: "
				<< waveLength << " < " << amplitudes_[0].l;
			Log::Warning(oss.str());

			return 0.0;
		}

		// get lower bound
		auto lower = --upper;
		const Amplitude& lowerAmp = *lower;
		if ((waveLength - lowerAmp.l) < WaveLengthAccuracy)
		{
			return lowerAmp.a;
		}

		// check that we're not higher than the last wave legth
		if (upper == amplitudes_.end())
		{
			std::ostringstream oss;
			oss << "SpectralColor: Wave length was out of range: "
				<< waveLength << " > " << amplitudes_[amplitudes_.size() - 1].l;
			Log::Warning(oss.str());

			return 0.0;
		}

		const Amplitude& upperAmp = *upper;

		// return linear interpolation of amplitude
		return lowerAmp.a
			+ ((waveLength - lowerAmp.l) / (upperAmp.l - lowerAmp.l)
			* (upperAmp.a - lowerAmp.a));
	}

	bool operator< (const double& lhs, const SpectralColor::Amplitude& rhs)
	{
		return lhs < rhs.l;
	}
}
