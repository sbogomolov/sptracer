#ifndef SPT_MATERIAL_H
#define SPT_MATERIAL_H

#include <memory>
#include <string>
#include "../Color/Color.h"

namespace SPTracer {

	class Material
	{
	public:
		virtual inline ~Material() { };

		virtual bool IsEmissive() const = 0;

	protected:
		inline Material() { };
	};

}

#endif
