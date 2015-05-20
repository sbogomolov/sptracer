#ifndef SPT_KD_TREE_NODE_H
#define SPT_KD_TREE_NODE_H

#include "../stdafx.h"

namespace SPTracer
{

	class KdTreeNode
	{
	public:
		KdTreeNode(unsigned int dimension, float position, std::unique_ptr<KdTreeNode> left, std::unique_ptr<KdTreeNode> right);
		virtual ~KdTreeNode();

		const KdTreeNode& left() const;
		const KdTreeNode& right() const;
		const unsigned int dimension() const;
		const float position() const;

	private:
		std::unique_ptr<KdTreeNode> left_;
		std::unique_ptr<KdTreeNode> right_;
		unsigned int dimension_;
		float position_;
	};

}

#endif
