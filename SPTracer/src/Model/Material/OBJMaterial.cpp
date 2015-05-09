#include "../../Exception.h"
#include "../../Intersection.h"
#include "../../Log.h"
#include "../../Ray.h"
#include "../../Spectrum.h"
#include "../../Util.h"
#include "../Model.h"
#include "../Color/Color.h"
#include "OBJMaterial.h"

namespace SPTracer
{

	OBJMaterial::OBJMaterial(std::unique_ptr<Color> diffuseReflectance, std::unique_ptr<Color> specularReflectance, float specularExponent, const Spectrum& spectrum)
		: PhongMaterial(std::move(diffuseReflectance), std::move(specularReflectance), specularExponent, spectrum)
	{
	}

	bool OBJMaterial::IsEmissive() const
	{
		return false;
	}

	void OBJMaterial::GetDiffuseReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const
	{
	}

	void OBJMaterial::GetSpecularReflectance(const Ray& ray, const Intersection& intersection, const Ray& newRay, std::vector<float>& reflectance) const
	{
	}

	void OBJMaterial::GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const
	{
	}

	float OBJMaterial::GetDiffuseReflectionProbability(int waveIndex) const
	{
		return 0.0f;
	}

	float OBJMaterial::GetSpecularReflectionProbability(int waveIndex) const
	{
		return 0.0f;
	}

}