#ifndef SPT_EXCEPTION_H
#define SPT_EXCEPTION_H

#include <exception>

namespace sptracer
{

	class Exception : public std::exception
	{
	public:
		Exception(const char* msg) : std::exception(msg) { };
	};

}

#endif
