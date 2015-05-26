#include "../stdafx.h"
#include "../Util.h"
#include "SplitPlane.h"
#include "SplitEvent.h"
#include "SplitEventType.h"

namespace SPTracer
{

	SplitEvent::SplitEvent(std::shared_ptr<Primitive> primitive, SplitPlane plane, SplitEventType type)
		: primitive_(std::move(primitive)), plane_(std::move(plane)), type_(type)
	{
	}

	const Primitive& SplitEvent::primitive() const
	{
		return *primitive_;
	}

	const SplitPlane& SplitEvent::plane() const
	{
		return plane_;
	}

	const SplitEventType& SplitEvent::type() const
	{
		return type_;
	}

	bool SplitEvent::operator<(const SplitEvent& b) const
	{
		if (std::abs(plane_.position - b.plane_.position) < Util::Eps)
		{
			return type_ < b.type_;
		}
		else
		{
			return plane_.position < b.plane_.position;
		}
	}

}
