#include "../stdafx.h"
#include "../Tracer/Intersection.h"
#include "../Tracer/Ray.h"
#include "../Util.h"
#include "../Vec3.h"
#include "Material.h"

namespace SPTracer
{

	void Material::GetNewRayDiffuse(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const
	{
		// NOTE: Importance sampling.
		// BDRF is 1/pi * cos(theta), it will be used as PDF
		// to prefer bright directions. Because the bright
		// directions are preferred in the choice of samples,
		// we do not have to weight them again by applying
		// the BDRF as a scaling factor to reflectance.
		// Scaling factor in this case is: BDRF/PDF = 1

		// z axis
		static const Vec3 zAxis(0.0f, 0.0f, 1.0f);

		// generate random ray direction using BDRF as PDF
		float phi = Util::RandFloat(0.0f, 2.0f * Util::Pi);
		float cosTheta = std::sqrt(Util::RandFloat(0.0f, 1.0f));
		newRay.direction = Vec3::FromPhiTheta(phi, cosTheta).RotateFromTo(zAxis, intersection.normal);

		// new ray origin is intersection point
		newRay.origin = intersection.point;

		// get diffuse reflectance
		GetDiffuseReflectance(ray, intersection, newRay, reflectance);
	}

}
