#ifndef KD_TREE_H
#define KD_TREE_H

#include "../stdafx.h"
#include "../Primitive/Triangle.h"

namespace SPTracer
{
	class Box;
	class KdTreeNode;
	class Primitive;
	class Scene;

	class KdTree
	{
	public:
		KdTree(std::vector<std::shared_ptr<Primitive>> model);
		virtual ~KdTree();

		const KdTreeNode& rootNode() const;

	private:
		static const float TraverseStepCost;
		static const float IntersectionCost;

		std::unique_ptr<KdTreeNode> rootNode_;

		std::unique_ptr<KdTreeNode> Build(std::vector<Primitive> primitives, const Box& box);
	};

}

#endif
