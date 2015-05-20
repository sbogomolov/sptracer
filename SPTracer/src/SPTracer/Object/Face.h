#ifndef SPT_FACE_H
#define SPT_FACE_H

#include "../stdafx.h"
#include "../Vec3.h"
#include "Vertex.h"

namespace SPTracer
{
	
	class Face
	{
	public:
		Face();
		Face(Vertex v1, Vertex v2, Vertex v3);
		virtual ~Face();

		Vertex& operator[](size_t index);
		const Vertex& operator[](size_t index) const;
		const Vec3& e1() const;
		const Vec3& e2() const;

	private:
		std::array<Vertex, 3> vertices_;
		Vec3 e1_;
		Vec3 e2_;

		void Init();
	};

}

#endif
