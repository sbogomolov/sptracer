#ifndef SPT_COLOR_H
#define SPT_COLOR_H

namespace SPTracer
{

	class Color
	{
	public:
		virtual ~Color() { };

		virtual float GetAmplitude(float waveLength) const = 0;

	protected:
		Color() { };
	};

}

#endif
