#include "../stdafx.h"
#include "Face.h"

namespace SPTracer
{

	Face::Face()
	{
		Init();
	}

	Face::Face(Vertex v1, Vertex v2, Vertex v3)
		: vertices_{ std::move(v1), std::move(v2), std::move(v3) }
	{
		Init();
	}

	Face::~Face()
	{
	}

	Vertex& SPTracer::Face::operator[](size_t index)
	{
		return vertices_[index];
	}

	const Vertex& SPTracer::Face::operator[](size_t index) const
	{
		return vertices_[index];
	}

	const Vec3& SPTracer::Face::e1() const
	{
		return e1_;
	}

	const Vec3& SPTracer::Face::e2() const
	{
		return e2_;
	}

	void SPTracer::Face::Init()
	{
		// pre-compute edges for ray-triangle intersection test
		e1_ = vertices_[1].coord - vertices_[0].coord;
		e2_ = vertices_[2].coord - vertices_[0].coord;
	}

}
