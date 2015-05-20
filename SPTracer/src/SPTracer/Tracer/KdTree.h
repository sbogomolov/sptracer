#ifndef KD_TREE_H
#define KD_TREE_H

namespace SPTracer
{
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

		// std::unique_ptr<KdTreeNode> Divide(const std::vector<Face> )

		std::unique_ptr<KdTreeNode> rootNode_;
	};

}

#endif
