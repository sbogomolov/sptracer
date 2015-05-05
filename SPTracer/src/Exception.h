#ifndef SPT_EXCEPTION_H
#define SPT_EXCEPTION_H

#include <exception>

namespace SPTracer
{

	class Exception : public std::exception
	{
	public:
		Exception(const char* msg) : std::exception(msg) { };
		Exception(std::string msg) : std::exception(msg.c_str()) { };
	};

}

#endif
