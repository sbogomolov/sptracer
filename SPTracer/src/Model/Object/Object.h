#ifndef SPT_OBJECT_H
#define SPT_OBJECT_H

#include <memory>

namespace SPTracer
{
	struct Intersection;
	struct Ray;
	class Material;

	class Object
	{
	public:
		virtual ~Object();

		virtual bool Intersect(const Ray& ray, Intersection& intersection) const = 0;
		bool GetNewRay(const Ray& ray, const Intersection& intersection, double waveLength, Ray& newRay, double& reflectance, double& bdrfPdf) const;
		bool IsEmissive() const;
		
	protected:
		static const double Eps;

		Object(std::string name, std::shared_ptr<Material> material);

		static Vec3 ComputeNormal(const Vec3& v1, const Vec3& v2, const Vec3& v3);
		static bool IntersectWithTriangle(const Ray& ray, const Vec3& v1, const Vec3& v2, const Vec3& v3, const Vec3& n, double d, Intersection& intersection);

	private:
		std::string name_;
		std::shared_ptr<Material> material_;

	};

}

#endif
