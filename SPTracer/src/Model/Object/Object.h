#ifndef SPT_OBJECT_H
#define SPT_OBJECT_H

#include <memory>
#include <vector>

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
		void GetNewRay(const Ray& ray, const Intersection& intersection, const Spectrum& spectrum, Ray& newRay, std::vector<float>& reflectance) const;
		void GetRadiance(const Ray& ray, const Intersection& intersection, const Spectrum& spectrum, std::vector<float>& radiance) const;
		bool IsEmissive() const;
		
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
