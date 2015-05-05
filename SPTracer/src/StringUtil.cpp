#include <algorithm>
#include <ctype.h>
#include <exception>
#include <sstream>
#include "StringUtil.h"

StringUtil::StringUtil()
{
}

// Converts string to lowercase.
std::string& StringUtil::ToLower(std::string& str)
{
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

// Trims characters from the beginning of string.
std::string& StringUtil::TrimBegin(std::string& str, const std::string& chars)
{
	size_t pos = str.find_first_not_of(chars);
	return str.erase(0, pos);
}

// Trims characters from the end of the string.
std::string& StringUtil::TrimEnd(std::string& str, const std::string& chars)
{
	size_t pos = str.find_last_not_of(chars);
	return str.erase(pos + 1);
}

// Trims characters from both the beginning and the end of the string.
std::string& StringUtil::Trim(std::string& str, const std::string& chars)
{
	return TrimEnd(TrimBegin(str, chars), chars);
}

int StringUtil::GetInt(const std::string & str)
{
	try
	{
		// try to get integer value
		return std::stoi(str.c_str());
	}
	catch (std::invalid_argument e)
	{
		throw std::exception(("Expected float, got: " + str).c_str());
	}
}

float StringUtil::GetFloat(const std::string& str)
{
	try
	{
		// try to get float value
		return std::stof(str.c_str());
	}
	catch (std::invalid_argument e)
	{
		throw std::exception(("Expected float, got: " + str).c_str());
	}
}

std::vector<float> StringUtil::GetFloatArray(const std::string& str, size_t count)
{
	std::vector<float> values;
	size_t prev = 0;
	size_t pos = 0;
	while (pos = str.find(';', pos + 1))
	{
		std::string token = str.substr(prev, pos - prev);
		
		try
		{
			// try to get float value
			values.push_back(std::stof(token.c_str()));
		}
		catch (std::invalid_argument e)
		{
			throw std::exception(("Expected float, got: " + token).c_str());
		}

		prev = pos + 1;
		if ((pos == str.npos) || (pos == (str.length() - 1)))
		{
			break;
		}
	}

	if (values.size() != count)
	{
		std::ostringstream oss;
		oss << "Wrong number of values, expected " << count << ", got " << values.size();
		throw std::exception(oss.str().c_str());
	}

	return values;
}
