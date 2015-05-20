#ifndef SPT_TRIANGLE_H
#define SPT_TRIANGLE_H

#include "../stdafx.h"
#include "../Vec3.h"
#include "Primitive.h"
#include "Vertex.h"

namespace SPTracer
{
	struct Intersection;
	struct Ray;
	class Material;

	class Triangle : public Primitive
	{
	public:
		explicit Triangle(std::shared_ptr<Material> material);
		Triangle(std::shared_ptr<Material> material, Vertex v1, Vertex v2, Vertex v3);
		virtual ~Triangle();

		Vertex& operator[](size_t index);
		const Vertex& operator[](size_t index) const;
		const Vec3& e1() const;
		const Vec3& e2() const;

		void ComputeNormals();
		virtual bool Intersect(const Ray& ray, Intersection& intersection) const override;

	private:
		std::array<Vertex, 3> vertices_;
		Vec3 e1_;
		Vec3 e2_;
	};

}

#endif
