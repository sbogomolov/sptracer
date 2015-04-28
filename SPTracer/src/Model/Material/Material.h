#ifndef SPT_MATERIAL_H
#define SPT_MATERIAL_H

#include <memory>
#include <string>
#include <vector>

namespace SPTracer
{

	struct Intersection;
	struct Spectrum;
	struct Ray;

	class Material
	{
	public:
		virtual ~Material() { };

		virtual void GetNewRay(const Ray& ray, const Intersection& intersection, const Spectrum& spectrum, Ray& newRay, std::vector<float>& reflectance) const = 0;
		virtual bool IsEmissive() const = 0;
		virtual void GetRadiance(const Ray& ray, const Intersection& intersection, const Spectrum& spectrum, std::vector<float>& radiance) const = 0;

	protected:
		Material() { };
	};

}

#endif
