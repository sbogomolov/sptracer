#ifndef SPT_MATERIAL_H
#define SPT_MATERIAL_H

#include "../../stdafx.h"

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
		virtual bool IsReflective() const = 0;
		virtual void GetNewRayDiffuse(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const;
		virtual bool GetNewRaySpecular(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const = 0;
		virtual void GetDiffuseReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const = 0;
		virtual void GetSpecularReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const = 0;
		virtual void GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const = 0;
		virtual float GetDiffuseReflectionProbability(int waveIndex) const = 0;			// use index -1 for average reflectivity
		virtual float GetSpecularReflectionProbability(int waveIndex) const = 0;		// use index -1 for average reflectivity

	protected:
		Material() { };
	};

}

#endif
