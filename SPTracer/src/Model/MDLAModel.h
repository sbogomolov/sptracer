#ifndef MDLA_MODEL_H
#define MDLA_MODEL_H

#include <string>
#include <vector>
#include "Model.h"

namespace SPTracer
{
	class Color;
	class SpectralColor;
	class ScalarColor;
	class Material;
	class LambertianMaterial;
	class PhongLuminaireMaterial;

	class MDLAModel : public Model
	{
	public:
		MDLAModel(std::string fileName);

	private:
		typedef std::vector<std::string> TokensList;
		typedef TokensList::const_iterator TokensIterator;

		static std::vector<std::string> GetTokens(std::string text);
		static void CheckToken(TokensIterator& it, TokensIterator& end);
		static bool IsEndToken(TokensIterator& it, TokensIterator& end);
		static void MustBeEndToken(TokensIterator& it, TokensIterator& end);
		static void CheckKeyword(TokensIterator& it, TokensIterator& end, std::string keyword);
			
		static std::string GetString(TokensIterator& it, TokensIterator& end);
		static double GetDouble(TokensIterator& it, TokensIterator& end);
		unsigned long GetInteger(TokensIterator& it, TokensIterator& end);

		void ParseTokens(const TokensList& tokens);
		void ParseCamera(TokensIterator& it, TokensIterator& end);

		std::unique_ptr<Color> ParseColorType(TokensIterator& it, TokensIterator& end);
		std::unique_ptr<SpectralColor> ParseSpectralColor(TokensIterator& it, TokensIterator& end);
		std::unique_ptr<ScalarColor> ParseScalarColor(TokensIterator& it, TokensIterator& end);

		std::string ParseMaterial(TokensIterator& it, TokensIterator& end);
		std::unique_ptr<Material> ParseMaterialType(TokensIterator& it, TokensIterator& end);
		std::shared_ptr<Material> ParseEmbeddedMaterial(TokensIterator& it, TokensIterator& end);
		std::unique_ptr<LambertianMaterial> ParseLambertianMaterial(TokensIterator& it, TokensIterator& end);
		std::unique_ptr<PhongLuminaireMaterial> ParsePhongLuminaireMaterial(TokensIterator& it, TokensIterator& end);

		void ParseVertexPositions(TokensIterator& it, TokensIterator& end, std::vector<Vec3>& vertices);
		void ParsePolygon(TokensIterator& it, TokensIterator& end, std::vector<unsigned long>& vertices);
		void ParseComplexPolygon(TokensIterator& it, TokensIterator& end,
			std::vector<unsigned long>& vertices, std::vector<std::vector<unsigned long>>& holes);

		void ParsePlanarMeshObject(TokensIterator& it, TokensIterator& end);
	};

}

#endif
