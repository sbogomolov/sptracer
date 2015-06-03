#ifndef KD_TREE_H
#define KD_TREE_H

#include "../stdafx.h"

namespace SPTracer
{
	struct SplitPlane;
	class Box;
	class KdTreeNode;
	class Primitive;
	class Scene;

	class KdTree
	{
	public:
		KdTree(std::vector<std::shared_ptr<Primitive>> primitives);
		virtual ~KdTree();

		const KdTreeNode& rootNode() const;

	private:
		struct Event
		{
			SplitPlane& plane;
			unsigned char type;
		};

		static const float TraverseStepCost;
		static const float IntersectionCost;

		std::shared_ptr<KdTreeNode> rootNode_;

		// recurent tree building procedure
		static std::shared_ptr<KdTreeNode> Build(Box box, std::vector<std::shared_ptr<Primitive>> primitives);

		// finds the best split plane
		static std::tuple<SplitPlane, float, bool> FindPlane(const Box& box, const std::vector<std::shared_ptr<Primitive>>& primitives);

		// splits the box with a plane
		static std::tuple<Box, Box> SplitBox(const Box& box, const SplitPlane& plane);
		
		// gets the cost of split using the Surface Area Heuristic and
		// selects the side where primitives lying exactly on plane should belong
		// (true - to the left side, false - to the right side)
		static std::tuple<float, bool> GetSurfaceAreaHeuristicCost(const Box& leftBox, const Box& rightBox, float surfaceArea, size_t leftCount, size_t planarCount, size_t rightCount);

		// gets the split cost
		static float GetCost(float leftProb, float rightProb, size_t leftCount, size_t rightCount);

		// finds neighbours for node and its child nodes
		void FindNeighbours(KdTreeNode& node);

		// find indirect neighbours of node
		void FindIndirectNeighbours(KdTreeNode& node, std::shared_ptr<KdTreeNode> searchNode, unsigned char face);
	};

}

#endif
