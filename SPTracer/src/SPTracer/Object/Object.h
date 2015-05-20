#ifndef SPT_OBJECT_H
#define SPT_OBJECT_H

#include "../stdafx.h"
#include "../Vec3.h"
#include "Face.h"

namespace SPTracer
{
	struct Intersection;
	struct Ray;
	struct Spectrum;
	class Face;
	class Material;

	class Object
	{
	public:
		Object(std::string name, std::shared_ptr<Material> material, std::vector<Face> faces, bool computeNormals);
		virtual ~Object();

		bool Intersect(const Ray& ray, Intersection& intersection) const;
		void GetNewRayDiffuse(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const;
		bool GetNewRaySpecular(const Ray& ray, const Intersection& intersection, Ray& newRay, std::vector<float>& reflectance) const;
		void GetRadiance(const Ray& ray, const Intersection& intersection, std::vector<float>& radiance) const;
		
		bool IsEmissive() const;
		bool IsReflective() const;
		float GetDiffuseReflectionProbability(int waveIndex) const;		// use index -1 for average reflectivity
		float GetSpecularReflectionProbability(int waveIndex) const;	// use index -1 for average reflectivity

	protected:
		static Vec3 ComputeNormal(const Vec3& v1, const Vec3& v2, const Vec3& v3);
		static bool IntersectWithTriangle(
			const Ray& ray,
			const Face& face,
			Intersection& intersection);

	private:
		std::string name_;
		std::shared_ptr<Material> material_;
		std::vector<Face> faces_;
	};

}

#endif
