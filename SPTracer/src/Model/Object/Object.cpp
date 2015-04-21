#include "Object.h"

namespace SPTracer
{
	Object::Object(std::string name, std::shared_ptr<Material> material)
		: name_(std::move(name)), material_(std::move(material))
	{
	}

	Object::~Object()
	{
	}

}
