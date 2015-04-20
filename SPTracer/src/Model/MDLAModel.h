#ifndef MDLA_MODEL_H
#define MDLA_MODEL_H

#include <string>
#include <vector>
#include "Model.h"
#include "Material/LambertianMaterial.h"
#include "Material/PhongLuminaireMaterial.h"
#include "Color/SpectralColor.h"

namespace SPTracer
{

	class MDLAModel : public Model
	{
	private:
		typedef std::vector<std::string> TokensList;

		static std::vector<std::string> GetTokens(std::string text);
		void ParseTokens(const TokensList& tokens);
		void ParseCamera(TokensList::const_iterator& it, TokensList::const_iterator& end);
		
		void ParseMaterial(TokensList::const_iterator& it, TokensList::const_iterator& end);
		std::unique_ptr<Material> ParseMaterialType(TokensList::const_iterator& it, TokensList::const_iterator& end);
		std::unique_ptr<LambertianMaterial> ParseLambertianMaterial(TokensList::const_iterator& it, TokensList::const_iterator& end);
		std::unique_ptr<PhongLuminaireMaterial> ParsePhongLuminaireMaterial(TokensList::const_iterator& it, TokensList::const_iterator& end);
		
		std::unique_ptr<Color> ParseColorType(TokensList::const_iterator& it, TokensList::const_iterator& end);
		std::unique_ptr<SpectralColor> ParseSpectralColor(TokensList::const_iterator& it, TokensList::const_iterator& end);

		static void CheckToken(TokensList::const_iterator& it, TokensList::const_iterator& end);
		static bool IsEndToken(TokensList::const_iterator& it, TokensList::const_iterator& end);
		static void MustBeEndToken(TokensList::const_iterator& it, TokensList::const_iterator& end);
		static std::string GetString(TokensList::const_iterator& it, TokensList::const_iterator& end);
		static double GetDouble(TokensList::const_iterator& it, TokensList::const_iterator& end);

	public:
		MDLAModel(std::string fileName);
	};

}

#endif
