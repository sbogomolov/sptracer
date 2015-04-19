#include <exception>
#include <fstream>
#include <sstream>
#include <vector>
#include "MDLAModel.h"
#include "../Exception.h"
#include "../Log.h"

namespace sptracer
{

	MDLAModel::MDLAModel(std::string fileName)
	{
		// open file
		std::ifstream file(fileName);
		if (!file)
		{
			std::string s("MDLAModel: Cannot open file: " + fileName);
			Log::Error(s);
			throw Exception(s.c_str());
		}

		// read file
		std::ostringstream ss;
		ss << file.rdbuf();

		// get tokens
		auto tokens = GetTokens(ss.str());

		// parse tokens
		ParseTokens(tokens);
	}

	MDLAModel::~MDLAModel()
	{
	}

	std::vector<std::string> MDLAModel::GetTokens(std::string text)
	{
		TokensList tokens;
		int start = -1;
		bool inStrToken = false;
		bool inComment = false;

		for (int i = 0; i < text.length(); i++)
		{
			// continue if inside a string token and current char is not '\"'
			if (inStrToken && (text[i] != '\"'))
			{
				continue;
			}

			// check if we're inside a comment
			if (inComment)
			{
				// comment ended
				if ((text[i] == '\r') || (text[i] == '\n'))
				{
					inComment = false;
				}

				// continue if inside a comment
				continue;
			}

			// store token '['
			if (text[i] == '[')
			{
				tokens.push_back(text.substr(i, 1));
			}

			switch (text[i])
			{
			case '%':
				// start comment
				inComment = true;

				// NOTE: fall through to store token

			case '\r':
			case '\n':
			case ' ':
			case '\t':
			case '[':
			case ']':
				// continue if we're not inside a token
				if (start == -1)
				{
					break;
				}

				// store current token
				tokens.push_back(text.substr(start, i - start));

				// reset token start
				start = -1;
				break;

			case '\"':
				// string token started or ended
				inStrToken = !inStrToken;
				
				// NOTE: fall through to default to start new token

			default:
				// set new token start
				if (start == -1)
				{
					start = i;
				}

				break;
			}

			// store token ']'
			if (text[i] == ']')
			{
				tokens.push_back(text.substr(i, 1));
			}
		}

		return tokens;
	}

	void MDLAModel::ParseTokens(const TokensList& tokens)
	{
		auto it = tokens.begin();
		
		// check file type
		if (*it != "mdlflA20")
		{
			auto s = "MDLAModel: Wrong file type";
			Log::Error(s);
			throw Exception(s);
		}

		auto end = tokens.end();
		while (it != end)
		{
			it++;
			const auto& token = *it;
			
			if (token == "cmr")
			{
				// camera keyword
				ParseCamera(it, end);
			}
			else
			{
				// unknown keyword
				std::string s = "MDLAModel: Unknown keyword: " + token;
				Log::Error(s);
				throw Exception(s.c_str());
			}
		}
	}

	void MDLAModel::CheckEnd(TokensList::const_iterator& it, TokensList::const_iterator& end)
	{
		// check if there is a token
		if (it == end)
		{
			auto s = "MDLAModel: No more tokens, expected end";
			Log::Error(s);
			throw Exception(s);
		}

		const auto& token = *it;
		if (token != "end")
		{
			std::string s = "MDLAModel: Bad token, expected end, got: " + token;
			Log::Error(s);
			throw Exception(s.c_str());
		}
	}

	std::string MDLAModel::GetString(TokensList::const_iterator& it, TokensList::const_iterator& end)
	{
		// check if there is a token
		if (it == end)
		{
			auto s = "MDLAModel: No more tokens, expected string";
			Log::Error(s);
			throw Exception(s);
		}

		const auto& token = *it;
		auto len = token.length();

		// check if this is a string token
		if ((token[0] != '\"') || (token[len - 1] != '\"'))
		{
			std::string s = "MDLAModel: Bad token, expected string, got: " + token;
			Log::Error(s);
			throw Exception(s.c_str());
		}

		return token.substr(1, len - 2);
	}

	double MDLAModel::GetDouble(TokensList::const_iterator& it, TokensList::const_iterator& end)
	{
		// check if there is a token
		if (it == end)
		{
			auto s = "MDLAModel: No more tokens, expected double";
			Log::Error(s);
			throw Exception(s);
		}

		const auto& token = *it;
		try
		{
			// try to get double token
			return std::stod(token);
		}
		catch (std::invalid_argument e)
		{
			std::string s = "MDLAModel: Bad token, expected double, got: " + token;
			Log::Error(s);
			throw Exception(s.c_str());
		}
	}

	void MDLAModel::ParseCamera(TokensList::const_iterator& it, TokensList::const_iterator& end)
	{
		camera_.name = GetString(++it, end);	// camera name
		
		camera_.p.x = GetDouble(++it, end);		// center of projection x
		camera_.p.y = GetDouble(++it, end);		// center of projection y
		camera_.p.z = GetDouble(++it, end);		// center of projection z

		camera_.n.x = GetDouble(++it, end);		// image plane normal x
		camera_.n.y = GetDouble(++it, end);		// image plane normal y
		camera_.n.z = GetDouble(++it, end);		// image plane normal z

		camera_.up.x = GetDouble(++it, end);	// up direction x
		camera_.up.y = GetDouble(++it, end);	// up direction y
		camera_.up.z = GetDouble(++it, end);	// up direction z

		camera_.f = GetDouble(++it, end);		// distance to image plane
		camera_.iw = GetDouble(++it, end);		// image width
		camera_.ih = GetDouble(++it, end);		// image height
		camera_.icx = GetDouble(++it, end);		// image center x
		camera_.icy = GetDouble(++it, end);		// image center y
		camera_.t = GetDouble(++it, end);		// time of exposure

		CheckEnd(++it, end);					// check end token
	}
}
