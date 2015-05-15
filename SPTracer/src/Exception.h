#ifndef SPT_EXCEPTION_H
#define SPT_EXCEPTION_H

#include <exception>
#include <string>

namespace SPTracer
{

	class Exception : public std::exception
	{
	public:
		explicit Exception(const char* msg) : std::exception(msg) { };
		explicit Exception(std::string msg) : std::exception(msg.c_str()) { };
	};

}

#endif
