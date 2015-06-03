#include "../stdafx.h"
#include "../Util.h"
#include "../Primitive/Primitive.h"
#include "KdTree.h"
#include "KdTreeNode.h"
#include "SplitEvent.h"
#include "SplitEventType.h"
#include "SplitPlane.h"

namespace SPTracer
{
	const float KdTree::TraverseStepCost = 0.3f;
	const float KdTree::IntersectionCost = 1.0f;

	KdTree::KdTree(std::vector<std::shared_ptr<Primitive>> primitives)
	{
		// get the bounding box for scene
		Vec3 min(std::numeric_limits<float>::max(), std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		Vec3 max(std::numeric_limits<float>::min(), std::numeric_limits<float>::min(), std::numeric_limits<float>::min());

		for (const auto& p : primitives)
		{
			Box box = p->GetBox();

			min[0] = std::min(min[0], box.min()[0]);
			max[0] = std::max(max[0], box.max()[0]);

			min[1] = std::min(min[1], box.min()[1]);
			max[1] = std::max(max[1], box.max()[1]);

			min[2] = std::min(min[2], box.min()[2]);
			max[2] = std::max(max[2], box.max()[2]);
		}

		// build kd-Tree
		rootNode_ = Build(Box(std::move(min), std::move(max)), std::move(primitives));

		// find neighbours
		if (!rootNode_->isLeaf())
		{
			FindNeighbours(*rootNode_->left_);
			FindNeighbours(*rootNode_->right_);
		}
	}

	KdTree::~KdTree()
	{
	}

	const KdTreeNode& KdTree::rootNode() const
	{
		return *rootNode_;
	}

	std::shared_ptr<KdTreeNode> KdTree::Build(Box box, std::vector<std::shared_ptr<Primitive>> primitives)
	{
		// return node if there are no primitives
		if (primitives.size() == 0)
		{
			return std::make_shared<KdTreeNode>(std::move(box), std::move(primitives));
		}

		// find best plane
		SplitPlane bestPlane;
		float bestCost;
		bool bestSide;
		std::tie(bestPlane, bestCost, bestSide) = FindPlane(box, primitives);
		
		// check if it makes sense to split
		if (bestCost > (IntersectionCost * primitives.size()))
		{
			// cost is too high, no more splitting
			return std::make_shared<KdTreeNode>(std::move(box), std::move(primitives));
		}

		// split the box
		Box left, right;
		std::tie(left, right) = SplitBox(box, bestPlane);

		// split primitives
		std::vector<std::shared_ptr<Primitive>> leftPrimitives;
		std::vector<std::shared_ptr<Primitive>> rightPrimitives;
		for (const auto& p : primitives)
		{
			Box b = p->GetBox();

			// check if primitive lies in split plane
			if (b.IsPlanar(bestPlane.dimension) && (std::abs(b.min()[bestPlane.dimension] - bestPlane.position) < Util::Eps))
			{
				// add primitive to the correct side
				if (bestSide)
				{
					leftPrimitives.push_back(p);
				}
				else
				{
					rightPrimitives.push_back(p);
				}

				continue;
			}

			// check left side
			if (b.min()[bestPlane.dimension] < bestPlane.position)
			{
				leftPrimitives.push_back(p);
			}

			// check right side
			if (b.max()[bestPlane.dimension] > bestPlane.position)
			{
				rightPrimitives.push_back(p);
			}
		}

		// return node with two recursively built child nodes
		return std::make_shared<KdTreeNode>(std::move(box), std::move(bestPlane), Build(left, leftPrimitives), Build(right, rightPrimitives));
	}

	std::tuple<SplitPlane, float, bool> KdTree::FindPlane(const Box& box, const std::vector<std::shared_ptr<Primitive>>& primitives)
	{
		// surface area
		float surfaceArea = box.GetSurfaceArea();

		// events list
		std::vector<SplitEvent> events;

		// best cost
		float bestCost = std::numeric_limits<float>::max();
		bool bestSide;
		SplitPlane bestPlane;

		// for all dimensions
		for (unsigned char dimension = 0; dimension < 3; dimension++)
		{
			// clear events for next dimension
			events.clear();

			// for all primitives
			for (const auto& p : primitives)
			{
				// get clipped box
				Box clippedBox = p->Clip(box);

				if (clippedBox.IsPlanar(dimension))
				{
					// add planar event
					events.emplace_back(p, SplitPlane{ dimension, clippedBox.min()[dimension] }, SplitEventType::Planar);
				}
				else
				{
					// add start event
					events.emplace_back(p, SplitPlane{ dimension, clippedBox.min()[dimension] }, SplitEventType::Start);

					// add end event
					events.emplace_back(p, SplitPlane{ dimension, clippedBox.max()[dimension] }, SplitEventType::End);
				}
			}

			// sort events
			std::sort(events.begin(), events.end());

			// number of events
			size_t n = events.size();

			// start with all triangles on the right
			size_t leftCount = 0;
			size_t planarCount = 0;
			size_t rightCount = primitives.size();

			// "sweep" plane over all split candidates
			size_t i = 0;
			while (i < n)
			{
				// plane position
				const SplitPlane& plane = events[i].plane();

				size_t start = 0;
				size_t end = 0;
				size_t planar = 0;

				// end events
				while ((i < n) && (std::abs(events[i].plane().position - plane.position) < Util::Eps) && (events[i].type() == SplitEventType::End))
				{
					end++;
					i++;
				}

				// planar events
				while ((i < n) && (std::abs(events[i].plane().position - plane.position) < Util::Eps) && (events[i].type() == SplitEventType::Planar))
				{
					planar++;
					i++;
				}

				// start events
				while ((i < n) && (std::abs(events[i].plane().position - plane.position) < Util::Eps) && (events[i].type() == SplitEventType::Start))
				{
					start++;
					i++;
				}

				// update triangles counts
				planarCount = planar;
				rightCount -= planar + end;

				// check that the plane is not one if the bounding planes
				if (((plane.position - box.min()[plane.dimension]) > Util::Eps) &&
					((box.max()[plane.dimension] - plane.position) > Util::Eps))
				{

					// split volume by plane
					Box left, right;
					std::tie(left, right) = SplitBox(box, plane);

					// get cost
					float cost;
					bool side;
					std::tie(cost, side) = GetSurfaceAreaHeuristicCost(left, right, surfaceArea, leftCount, planarCount, rightCount);

					// update best cost
					if (cost < bestCost)
					{
						bestCost = cost;
						bestSide = side;
						bestPlane = plane;
					}
				}

				leftCount += start;
				leftCount += planar;
				rightCount -= planar;
				planarCount = 0;
			}
		}

		return std::make_tuple(bestPlane, bestCost, bestSide);
	}

	std::tuple<Box, Box> KdTree::SplitBox(const Box& box, const SplitPlane& plane)
	{
		// copy dimenstions from the original box
		Vec3 leftMin = box.min();
		Vec3 leftMax = box.max();
		Vec3 rightMin = box.min();
		Vec3 rightMax = box.max();

		// modify corresponding dimension
		leftMax[plane.dimension] = plane.position;
		rightMin[plane.dimension] = plane.position;

		// return tuple
		return std::make_tuple(
			Box(std::move(leftMin), std::move(leftMax)),
			Box(std::move(rightMin), std::move(rightMax)));
	}

	std::tuple<float, bool> KdTree::GetSurfaceAreaHeuristicCost(const Box& leftBox, const Box& rightBox, float surfaceArea, size_t leftCount, size_t planarCount, size_t rightCount)
	{
		// get probabilities
		float leftProb = leftBox.GetSurfaceArea() / surfaceArea;
		float rightProb = rightBox.GetSurfaceArea() / surfaceArea;

		// get costs
		float costLeft = GetCost(leftProb, rightProb, leftCount + planarCount, rightCount);
		float costRight = GetCost(leftProb, rightProb, leftCount, rightCount + planarCount);
		
		// return the best cost
		if (costLeft < costRight)
		{
			return std::make_tuple(costLeft, true);
		}
		else
		{
			return std::make_tuple(costRight, false);
		}
	}

	float KdTree::GetCost(float leftProb, float rightProb, size_t leftCount, size_t rightCount)
	{
		float k = (leftCount == 0) || (rightCount == 0) ? 0.8f : 1.0f;
		return k * (TraverseStepCost + IntersectionCost * (leftProb * leftCount + rightProb * rightCount));
	}

	void KdTree::FindNeighbours(KdTreeNode& node)
	{
		// check if node is leaf
		if (node.isLeaf())
		{
			// iterate over all faces
			for (unsigned char i = 0; i < 6; i++)
			{
				// flag indicating that the current face is "left"
				// (corresponding coordinate is smaller)
				bool left = (i % 2) == 0;
				
				// current dimension
				unsigned char dimension = i / 2;

				// face position
				float pos = (left ? node.box().min() : node.box().max())[dimension];

				// current neighbour candidate node
				std::shared_ptr<KdTreeNode> candidate = rootNode_;

				// find neighbour in the tree
				while (true)
				{
					// check if node is leaf
					if (candidate->isLeaf())
					{
						// position of candidate neighbour node face
						float candidatePos = (left ? candidate->box().max() : candidate->box().min())[dimension];

						// check if the node is neighbour
						if (std::abs(pos - candidatePos) < Util::Eps)
						{
							// add neighbour
							node.neighbours_[i].push_back(candidate);
						}

						// there are no more nodes to check
						break;
					}
					else
					{
						// split plane
						const SplitPlane& plane = candidate->plane();

						// check the dimension of split plane
						if (plane.dimension != dimension)
						{
							// check if it splits the face
							if ((plane.position > (node.box().min()[plane.dimension] + Util::Eps)) &&
								(plane.position < (node.box().max()[plane.dimension] - Util::Eps)))
							{
								// plane splits the face
								// search the tree to find indirect neighbours
								FindIndirectNeighbours(node, rootNode_, i);
								
								// no need to check other nodes
								break;
							}
						}
						else
						{
							// check if face is in the split plane
							if (std::abs(pos - plane.position) < Util::Eps)
							{
								// face is in the split plane, both sub-trees contain the face
								// select the subtree that does not contain current node
								candidate = left ? candidate->left_ : candidate->right_;
								continue;
							}
						}

						// select the subtree that contains the whole face
						candidate = plane.position > candidate->box().max()[plane.dimension] ? candidate->left_ : candidate->right_;
						continue;
					}
				}
			}

		}
		else
		{
			// find neighbours for left and right child nodes
			FindNeighbours(*node.left_);
			FindNeighbours(*node.right_);
		}
	}

	void KdTree::FindIndirectNeighbours(KdTreeNode& node, std::shared_ptr<KdTreeNode> searchNode, unsigned char face)
	{
		// flag indicating that the current face is "left"
		// (corresponding coordinate is smaller)
		bool left = (face % 2) == 0;

		// current dimension
		unsigned char dimension = face / 2;

		// face position
		float pos = (left ? node.box().min() : node.box().max())[dimension];

		if (searchNode->isLeaf())
		{
			// position of candidate neighbour node face
			float candidatePos = (left ? searchNode->box().max() : searchNode->box().min())[dimension];

			// check if node is a neighbour
			if (std::abs(pos - candidatePos) < Util::Eps)
			{
				// add neighbour node
				node.neighbours_[face].push_back(searchNode);
			}
		}
		else
		{
			// split plane
			const SplitPlane& plane = searchNode->plane();

			// check split plane dimension
			if (dimension != plane.dimension)
			{
				// select corresponding sub-tree
				FindIndirectNeighbours(node, plane.position > pos ? searchNode->left_ : searchNode->right_, face);
			}
			else
			{
				// analyze the position of split plane
				if (plane.position < (node.box().min()[dimension] + Util::Eps))
				{
					// split plane is on the left, so we select the right sub-tree
					FindIndirectNeighbours(node, searchNode->right_, face);
				}
				else if (plane.position > (node.box().max()[dimension] - Util::Eps))
				{
					// split plane is on the right, so we select the left sub-tree
					FindIndirectNeighbours(node, searchNode->left_, face);
				}
				else
				{
					// split plane splits the face, we need both sub-trees
					FindIndirectNeighbours(node, searchNode->left_, face);
					FindIndirectNeighbours(node, searchNode->right_, face);
				}
			}
		}
	}

}
