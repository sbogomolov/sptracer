#ifndef MDLA_MODEL_H
#define MDLA_MODEL_H

#include <string>
#include <vector>
#include "Model.h"

namespace sptracer
{

	class MDLAModel : public Model
	{
	private:
		typedef std::vector<std::string> TokensList;

		static std::vector<std::string> GetTokens(std::string text);
		void ParseTokens(const TokensList& tokens);
		void ParseCamera(TokensList::const_iterator& it, TokensList::const_iterator& end);

		static void CheckEnd(TokensList::const_iterator& it, TokensList::const_iterator& end);
		static std::string GetString(TokensList::const_iterator& it, TokensList::const_iterator& end);
		static double GetDouble(TokensList::const_iterator& it, TokensList::const_iterator& end);

	public:
		MDLAModel(std::string fileName);
		virtual ~MDLAModel();
	};

}

#endif
