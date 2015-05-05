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

		virtual bool IsEmissive() const = 0;
		virtual void GetNewRay(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const = 0;
		virtual void GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const = 0;
		virtual float GetDiffuseReflectivity(int waveIndex) const = 0;		// use index -1 for average reflectivity
		virtual float GetSpecularReflectivity(int waveIndex) const = 0;		// use index -1 for average reflectivity

	protected:
		Material() { };
	};

}

#endif
