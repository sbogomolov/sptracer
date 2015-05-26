#ifndef SPT_SPLIT_EVENT_H
#define SPT_SPLIT_EVENT_H

#include "../stdafx.h"
#include "SplitPlane.h"

namespace SPTracer
{
	enum class SplitEventType;
	class Primitive;

	class SplitEvent
	{
	public:
		SplitEvent(std::shared_ptr<Primitive> primitive, SplitPlane plane, SplitEventType type);

		const Primitive& primitive() const;
		const SplitPlane& plane() const;
		const SplitEventType& type() const;

		bool operator<(const SplitEvent& b) const;

	private:
		std::shared_ptr<Primitive> primitive_;
		SplitPlane plane_;
		SplitEventType type_;
	};

}

#endif
