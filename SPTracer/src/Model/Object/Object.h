#ifndef SPT_OBJECT_H
#define SPT_OBJECT_H

#include <memory>
#include "../Material/Material.h"

namespace SPTracer
{

	class Object
	{
	public:
		virtual ~Object();

	protected:
		Object(std::string name, std::shared_ptr<Material> material);

	private:
		std::string name_;
		std::shared_ptr<Material> material_;

	};

}

#endif
