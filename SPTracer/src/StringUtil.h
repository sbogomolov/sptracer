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
		static std::string& Replace(std::string& str, const std::string& replace, const std::string& with);
		static std::vector<std::string> Split(const std::string& str, char delim);

		static long GetInt(const std::string& str);
		static float GetFloat(const std::string& str);
		static std::vector<float> GetFloatArray(const std::string& str, size_t count, char delim);
		static std::vector<float> GetFloatArray(const std::string& str, size_t minCount, size_t maxCount, char delim);

	private:
		StringUtil();
	};

}

#endif
