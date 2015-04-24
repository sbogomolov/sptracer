#ifndef SPT_MATERIAL_H
#define SPT_MATERIAL_H

#include <memory>
#include <string>

namespace SPTracer
{

	struct Intersection;
	struct Ray;

	class Material
	{
	public:
		virtual ~Material() { };

		virtual bool IsEmissive() const = 0;
		virtual bool GetNewRay(const Ray& ray, const Intersection& intersection, double waveLength, Ray& newRay, double& reflectance, double& bdrfPdf) const = 0;

	protected:
		Material() { };
	};

}

#endif
