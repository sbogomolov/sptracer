#ifndef SPT_UTIL_H
#define SPT_UTIL_H

namespace SPTracer
{

	class Util
	{
	public:
		static const float Eps;
		static const float Pi;

		static float RandFloat(float min, float max);

	private:
		Util();
	};

}

#endif
