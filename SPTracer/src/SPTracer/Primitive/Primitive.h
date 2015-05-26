#ifndef SPT_PRIMITIVE_H
#define SPT_PRIMITIVE_H

#include "../stdafx.h"

namespace SPTracer
{
	struct Intersection;
	struct Ray;
	class Box;
	class Material;

	class Primitive
	{
	public:
		virtual ~Primitive();

		const Material& material() const;
		virtual const Box GetBox() const = 0;
		virtual bool Intersect(const Ray& ray, Intersection& intersection) const = 0;
		virtual Box Clip(const Box& box) const = 0;

	protected:
		explicit Primitive(std::shared_ptr<Material> material);

	private:
		std::shared_ptr<Material> material_;
	};

}

#endif
