#ifndef SPT_MATERIAL_H
#define SPT_MATERIAL_H

#include <memory>
#include <string>

namespace SPTracer
{

	struct Intersection;
	struct Ray;
	struct WeightFactors;

	class Material
	{
	public:
		virtual ~Material() { };

		virtual void GetNewRay(const Ray& ray, const Intersection& intersection, float waveLength, Ray& newRay, WeightFactors& weightFactors) const = 0;
		virtual bool IsEmissive() const = 0;
		virtual float GetRadiance(const Ray& ray, const Intersection& intersection, float waveLength) const = 0;

	protected:
		Material() { };
	};

}

#endif
