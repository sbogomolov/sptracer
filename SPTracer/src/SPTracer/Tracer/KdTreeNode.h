#ifndef SPT_KD_TREE_NODE_H
#define SPT_KD_TREE_NODE_H

#include "../stdafx.h"
#include "../Object/Box.h"

namespace SPTracer
{

	class KdTreeNode
	{
	public:
		KdTreeNode(Box box, std::vector<Face> faces);
		KdTreeNode(Box box, std::unique_ptr<KdTreeNode> left, std::unique_ptr<KdTreeNode> right);
		virtual ~KdTreeNode();

		const KdTreeNode& left() const;
		const KdTreeNode& right() const;
		const Box& box() const;

	private:
		std::vector<Face> faces_;
		std::unique_ptr<KdTreeNode> left_;
		std::unique_ptr<KdTreeNode> right_;
		Box box_;
	};

}

#endif
