#include "../stdafx.h"
#include "../Exception.h"
#include "../Util.h"
#include "../Primitive/Primitive.h"
#include "../Tracer/Intersection.h"
#include "../Tracer/Ray.h"
#include "KDTree.h"
#include "KDTreeNode.h"
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
		// move primitives vector, because it will not be used in the future
		kdTree_ = std::make_unique<KdTree>(std::move(primitives_));
	}

	bool Scene::Intersect(const Ray& ray, Intersection& intersection) const
	{
		//// set initial intersection distance to max possible,
		//// so that any intersection will be closer than that
		//intersection.distance = std::numeric_limits<float>::max();

		//Intersection newIntersection;

		//for (const auto& p : primitives_)
		//{
		//	// find new intersection
		//	bool success = p->Intersect(ray, newIntersection);
		//	
		//	// check if new intersection is closer
		//	if (success && (newIntersection.distance < intersection.distance))
		//	{
		//		intersection = newIntersection;
		//		intersection.primitive = p;
		//	}
		//}

		//return intersection.distance < std::numeric_limits<float>::max();

		// ray inverted direction
		const Vec3 invDirection = 1 / ray.direction;

		// find the first box that ray intersects
		float tnear, tfar;
		const KdTreeNode* node = FindFirstIntersection(ray, invDirection, tnear, tfar);
		if (!node)
		{
			// no intersection with scene
			return false;
		}

		// set initial intersection distance to max possible,
		// so that any intersection will be closer than that
		intersection.distance = std::numeric_limits<float>::max();

		Intersection newIntersection;

		// find intersection with primitive
		while (true)
		{
			// find intersections with primitives in the node
			for (const auto& p : node->primitives())
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

			// check if intersection was found
			if (intersection.distance < std::numeric_limits<float>::max())
			{
				return true;
			}

			// Intersection was not found,
			// get the node where ray travels next.
			node = FindNextIntersection(node, ray, invDirection, tnear, tfar);
			if (node == nullptr)
			{
				// no next node - no intersection
				return false;
			}
		}

		return false;
	}

	const KdTreeNode* Scene::FindFirstIntersection(const Ray& ray, const Vec3& invDirection, float& tnear, float& tfar) const
	{
		// start with root node
		const KdTreeNode* node = &kdTree_->rootNode();

		// check if ray intersects the scene
		if (!node->box().Intersect(ray, invDirection, tnear, tfar))
		{
			return nullptr;
		}

		// find the first box that ray intersects
		while (true)
		{
			// check if node is leaf
			if (node->isLeaf())
			{
				// the smallest first box interseced by the ray is found
				return node;
			}

			// test intersection with left sub-box
			float tnearLeft, tfarLeft;
			bool left = node->left().box().Intersect(ray, invDirection, tnearLeft, tfarLeft);

			// test intersection with right sub-box
			float tnearRight, tfarRight;
			bool right = node->right().box().Intersect(ray, invDirection, tnearRight, tfarRight);

			// check what sub-boxes were intersected
			if (left && right)
			{
				// both sub-boxes were intersected
				if (std::abs(tnearLeft - tnearRight) < Util::Eps)
				{
					// special case when ray hits exactly between the sub-boxes
					// choose the sub-box in which far intersection point is further
					if (tfarLeft > tfarRight)
					{
						// select left sub-box
						node = &node->left();
						tnear = tnearLeft;
						tfar = tfarLeft;
					}
					else
					{
						// select right sub-box
						node = &node->right();
						tnear = tnearRight;
						tfar = tfarRight;
					}
				}
				else if (tnearLeft < tnearRight)
				{
					// select left sub-box
					node = &node->left();
					tnear = tnearLeft;
					tfar = tfarLeft;
				}
				else
				{
					// select right sub-box
					node = &node->right();
					tnear = tnearRight;
					tfar = tfarRight;
				}
			}
			else if (left)
			{
				// select left sub-box
				node = &node->left();
				tnear = tnearLeft;
				tfar = tfarLeft;
			}
			else
			{
				// select right sub-box
				node = &node->right();
				tnear = tnearRight;
				tfar = tfarRight;
			}
		}

		// should never get here
		throw Exception("Somehow escaped infinite loop in Scene::FindFirstIntersection");
	}

	const KdTreeNode* Scene::FindNextIntersection(const KdTreeNode* node, const Ray& ray, const Vec3& invDirection, float& tnear, float& tfar) const
	{
		// get the far point of intersection
		Vec3 far = ray.origin + tfar * ray.direction;

		// store original tfar to avoid infinite loop between two neighbours
		float tfarOriginal = tfar;

		// set initial value for tnear equal to tfar
		tnear = tfar;

		// next node
		KdTreeNode* nextNode = nullptr;

		// check all dimensions
		for (size_t i = 0; i < 3; ++i)
		{
			int face = -1;
			if (std::abs(far[i] - node->box().min()[i]) < Util::Eps)
			{
				// left plane in the current dimension
				face = i * 2;
			}
			else if (std::abs(far[i] - node->box().max()[i]) < Util::Eps)
			{
				// right plane in the current dimension
				face = i * 2 + 1;
			}

			if (face == -1)
			{
				// no faces
				continue;
			}

			// check if intersection point lies on any of the neighbour cells
			for (const auto& n : node->neighbours()[face])
			{
				bool found = true;
				for (size_t j = 0; j < 3; ++j)
				{
					// skip current dimension
					if (i == j)
					{
						continue;
					}

					if ((far[j] < node->box().min()[j]) || ((far[j] > node->box().max()[j])))
					{
						found = false;
						break;
					}
				}

				// check if neighbour was found
				if (found)
				{
					// intersect ray with neigbour
					float tn, tf;
					if (!n->box().Intersect(ray, invDirection, tn, tf))
					{
						// some mistake, no intersection
						continue;
					}

					// this check is needed to select the right neighbour if ray hits
					// exactly in the border between two neighbours
					if (((tf - tn) > (tfar - tnear)) && (tn > (tfarOriginal - Util::Eps)))
					{
						// store found neighbour
						nextNode = n.get();
						tnear = tn;
						tfar = tf;
					}
				}
			}
		}

		return nextNode;
	}

}
