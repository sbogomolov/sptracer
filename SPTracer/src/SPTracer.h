#ifndef SPT_SPTRACER_H
#define SPT_SPTRACER_H

#include <memory>
#include <string>
#include "Model/Model.h"

namespace SPTracer
{

	class SPTracer
	{
	public:
		SPTracer(std::string fileName);
		virtual ~SPTracer();

	private:
		std::unique_ptr<Model> model_;
	};

}

#endif
