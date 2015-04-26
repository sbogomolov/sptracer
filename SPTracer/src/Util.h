#ifndef SPT_UTIL_H
#define SPT_UTIL_H

namespace SPTracer
{

	class Util
	{
	public:
		static const double Eps;
		static const double Pi;

		static double RandDouble(double min, double max);

	private:
		Util();
	};

}

#endif
