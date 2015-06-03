#ifndef SPT_BOX_H
#define SPT_BOX_H

#include "../stdafx.h"
#include "../Vec3.h"

namespace SPTracer
{

	struct Intersection;
	struct Ray;

	class Box
	{
	public:
		Box();
		Box(Vec3 min, Vec3 max);

		const Vec3& min() const;
		const Vec3& max() const;

		const float GetSurfaceArea() const;
		bool Intersect(const Ray& ray, const Vec3& invDirection, Vec3& near, Vec3& far);
		bool IsPlanar(unsigned char dimension) const;

	private:
		Vec3 min_;	// lower limit for coordinates
		Vec3 max_;	// upper limit for coordinates
	};

}

#endif
