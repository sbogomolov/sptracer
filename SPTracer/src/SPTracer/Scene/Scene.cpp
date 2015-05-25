#include "../stdafx.h"
#include "../Util.h"
#include "../Primitive/Primitive.h"
#include "../Tracer/Intersection.h"
#include "../Tracer/Ray.h"
#include "KDTree.h"
#include "Scene.h"

namespace SPTracer
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::BuildKdTree()
	{
		// move primitoves vector, because it will not be used in the future
		kdTree_ = std::make_unique<KdTree>(std::move(primitives_));
	}

	bool Scene::Intersect(const Ray& ray, Intersection& intersection) const
	{
		// set initial intersection distance to max possible,
		// so that any intersection will be closer than that
		intersection.distance = std::numeric_limits<float>::infinity();

		Intersection newIntersection;

		for (const auto& p : primitives_)
		{
			// find new intersection
			bool success = p->Intersect(ray, newIntersection);
			
			// check if new intersection is closer
			if (success && (newIntersection.distance < intersection.distance))
			{
				intersection = newIntersection;
				intersection.primitive = p;
			}
		}

		return intersection.distance < std::numeric_limits<float>::infinity();
	}

}
