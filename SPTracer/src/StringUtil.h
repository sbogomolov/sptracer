#ifndef STRING_UTIL_H
#define STRING_UTIL_H

#include <string>
#include <vector>

namespace SPTracer 
{

	class StringUtil
	{
	public:
		static std::string& ToLower(std::string& str);
		static std::string& TrimBegin(std::string& str, const std::string& chars);
		static std::string& TrimEnd(std::string& str, const std::string& chars);
		static std::string& Trim(std::string& str, const std::string& chars);

		static int GetInt(const std::string& str);
		static float GetFloat(const std::string& str);
		static std::vector<float> GetFloatArray(const std::string& str, int count, char delim);

	private:
		StringUtil();
	};

}

#endif
