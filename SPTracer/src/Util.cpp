#include <chrono>
#include <random>
#include "Util.h"

namespace SPTracer
{

	const double Util::Eps = 1e-10;
	const double Util::Pi = 3.14159265358979323846;

	double Util::RandDouble(double min, double max)
	{
		static thread_local std::mt19937 generator((unsigned int)std::chrono::high_resolution_clock::now().time_since_epoch().count());
		std::uniform_real_distribution<double> distribution(min, max);
		return distribution(generator);
	}

}
