#ifndef SPT_MATERIAL_H
#define SPT_MATERIAL_H

#include <memory>
#include <string>
#include "../Color/Color.h"

namespace SPTracer {

	class Material
	{
	public:
		virtual ~Material() { };

		virtual bool IsEmissive() const = 0;

	protected:
		Material() { };
	};

}

#endif
