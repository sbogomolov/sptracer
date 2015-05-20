#include "../stdafx.h"
#include "Primitive.h"

namespace SPTracer
{
	Primitive::Primitive(std::shared_ptr<Material> material)
		: material_(std::move(material))
	{
	}

	Primitive::~Primitive()
	{
	}

	const Material& Primitive::material() const
	{
		return *material_;
	}

}
