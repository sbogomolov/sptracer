#ifndef SPT_PHONG_LUMINAIRE_MATERIAL_H
#define SPT_PHONG_LUMINAIRE_MATERIAL_H

#include "../stdafx.h"
#include "Material.h"

namespace SPTracer
{

	class Color;

	class PhongLuminaireMaterial : public Material
	{
	public:
		PhongLuminaireMaterial(
			std::unique_ptr<Material> reflectiveMaterial,
			std::unique_ptr<Color> radiance,
			float phongExponent,
			const Spectrum& spectrum);

		virtual bool IsEmissive() const override;
		virtual bool IsReflective() const override;
		virtual void GetNewRayDiffuse(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const override;
		virtual bool GetNewRaySpecular(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const override;
		virtual void GetDiffuseReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const override;
		virtual void GetSpecularReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const override;
		virtual void GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const override;
		virtual float GetDiffuseReflectionProbability(int waveIndex) const override;		// use index -1 for average reflectivity
		virtual float GetSpecularReflectionProbability(int waveIndex) const override;		// use index -1 for average reflectivity

	private:
		std::unique_ptr<Material> reflectiveMaterial_;
		std::unique_ptr<Color> radiance_;
		float phongExponent_;
		std::vector<float> precomputedRadiance_;
		bool reflective_;
		bool phongExponentUsed_;
	};

}

#endif
