#ifndef SPT_MATERIAL_H
#define SPT_MATERIAL_H

#include <memory>
#include <string>
#include "../../Intersection.h"
#include "../../Ray.h"
#include "../Color/Color.h"

namespace SPTracer {

	class Material
	{
	public:
		virtual ~Material() { };

		virtual bool IsEmissive() const = 0;
		virtual bool GetNewRay(const Ray& ray, const Intersection& intersection, Ray& newRay) const = 0;

	protected:
		Material() { };
	};

}

#endif
