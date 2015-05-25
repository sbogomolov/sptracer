#ifndef KD_TREE_H
#define KD_TREE_H

#include "../stdafx.h"
#include "../Primitive/Triangle.h"

namespace SPTracer
{
	class Box;
	class KdTreeNode;
	class Scene;

	class KdTree
	{
	public:
		KdTree(const Scene& model);
		virtual ~KdTree();

		const KdTreeNode& rootNode() const;

	private:
		static const float TraverseStepCost;
		static const float IntersectionCost;

		std::unique_ptr<KdTreeNode> Build(std::vector<Triangle> primitives, const Box& box);

		std::unique_ptr<KdTreeNode> rootNode_;
	};

}

#endif
