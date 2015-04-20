#ifndef SPT_COLOR_H
#define SPT_COLOR_H

namespace SPTracer
{

	class Color
	{
	public:
		virtual inline ~Color() { };

		virtual double GetAmplitude(double waveLength) const = 0;

	protected:
		inline Color() { };
	};

}

#endif
