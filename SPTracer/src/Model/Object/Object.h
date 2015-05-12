#ifndef SPT_OBJECT_H
#define SPT_OBJECT_H

#include <memory>
#include <vector>
#include "../../Vec3.h"

namespace SPTracer
{
	struct Intersection;
	struct Ray;
	struct Spectrum;
	class Material;

	class Object
	{
	public:
		virtual ~Object();

		virtual bool Intersect(const Ray& ray, Intersection& intersection) const = 0;
		void GetNewRayDiffuse(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const;
		bool GetNewRaySpecular(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const;
		void GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const;
		
		bool IsEmissive() const;
		bool IsReflective() const;
		float GetDiffuseReflectionProbability(int waveIndex) const;		// use index -1 for average reflectivity
		float GetSpecularReflectionProbability(int waveIndex) const;	// use index -1 for average reflectivity

	protected:
		Object(std::string name, std::shared_ptr<Material> material);

		static Vec3 ComputeNormal(const Vec3& v1, const Vec3& v2, const Vec3& v3);
		static bool IntersectWithTriangle(const Ray& ray, const Vec3& n, const Vec3& v1, const Vec3& v2, const Vec3& v3, Intersection& intersection);

	private:
		std::string name_;
		std::shared_ptr<Material> material_;

	};

}

#endif
