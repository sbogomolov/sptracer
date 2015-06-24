#ifndef SPT_EXCEPTION_H
#define SPT_EXCEPTION_H

#include "stdafx.h"

namespace SPTracer
{

	class Exception : public std::runtime_error
	{
	public:
		explicit Exception(const char* msg) : std::runtime_error(msg) { };
		explicit Exception(std::string msg) : std::runtime_error(msg.c_str()) { };
	};

}

#endif
