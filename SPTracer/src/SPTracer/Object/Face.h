#ifndef SPT_FACE_H
#define SPT_FACE_H

#include "../stdafx.h"
#include "../Vec3.h"
#include "Vertex.h"

namespace SPTracer
{
	struct Intersection;
	struct Ray;
	class Material;

	class Face
	{
	public:
		explicit Face(std::shared_ptr<Material> material);
		Face(std::shared_ptr<Material> material, Vertex v1, Vertex v2, Vertex v3);
		virtual ~Face();

		Vertex& operator[](size_t index);
		const Vertex& operator[](size_t index) const;
		const Material& material() const;
		const Vec3& e1() const;
		const Vec3& e2() const;

		void ComputeNormals();
		bool Intersect(const Ray& ray, Intersection& intersection) const;

	private:
		std::array<Vertex, 3> vertices_;
		std::shared_ptr<Material> material_;
		Vec3 e1_;
		Vec3 e2_;
	};

}

#endif
