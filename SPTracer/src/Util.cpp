#include <chrono>
#include <random>
#include "Util.h"

namespace SPTracer
{

	const float Util::Eps = 1e-5f;
	const float Util::Pi = 3.14159265358979323846f;

	float Util::RandFloat(float min, float max)
	{
		static thread_local std::mt19937 generator(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(generator);
	}

}
