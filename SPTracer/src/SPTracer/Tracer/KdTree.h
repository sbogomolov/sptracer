#ifndef KD_TREE_H
#define KD_TREE_H

#include "../stdafx.h"
#include "../Object/Face.h"

namespace SPTracer
{
	class Box;
	class KdTreeNode;
	class Model;

	class KdTree
	{
	public:
		KdTree(const Model& model);
		virtual ~KdTree();

		const KdTreeNode& rootNode() const;

	private:
		static const float TraverseStepCost;
		static const float IntersectionCost;

		std::unique_ptr<KdTreeNode> Build(std::vector<Face> faces, const Box& box);

		std::unique_ptr<KdTreeNode> rootNode_;
	};

}

#endif
