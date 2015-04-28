#include <chrono>
#include <random>
#include "Util.h"

namespace SPTracer
{

	const double Util::Eps = 1e-8;
	const double Util::Pi = 3.14159265358979323846;

	double Util::RandFloat(double min, double max)
	{
		static thread_local std::mt19937 generator(static_cast<unsigned int>(std::chrono::high_resolution_clock::now().time_since_epoch().count()));
		std::uniform_real_distribution<double> distribution(min, max);
		return distribution(generator);
	}

}
