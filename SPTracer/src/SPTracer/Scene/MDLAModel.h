#ifndef SPT_MDLA_MODEL_H
#define SPT_MDLA_MODEL_H

#include "../stdafx.h"
#include "../Vec3.h"

namespace SPTracer
{
	struct Camera;
	struct Spectrum;
	class Color;
	class SpectralColor;
	class ScalarColor;
	class Material;
	class LambertianMaterial;
	class PhongLuminaireMaterial;
	class Scene;

	class MDLAModel
	{
	public:
		static std::unique_ptr<Scene> Load(std::string fileName, const Spectrum& spectrum, Camera& camera);

	private:
		typedef std::vector<std::string> TokensList;
		typedef TokensList::const_iterator TokensIterator;

		static std::unique_ptr<Scene> scene_;

		MDLAModel();

		static std::vector<std::string> GetTokens(std::string text);
		static void CheckToken(TokensIterator& it, TokensIterator& end);
		static bool IsEndToken(TokensIterator& it, TokensIterator& end);
		static void MustBeEndToken(TokensIterator& it, TokensIterator& end);
		static void CheckKeyword(TokensIterator& it, TokensIterator& end, std::string keyword);
			
		static std::string GetString(TokensIterator& it, TokensIterator& end);
		static float GetFloat(TokensIterator& it, TokensIterator& end);
		static unsigned long GetInteger(TokensIterator& it, TokensIterator& end);

		static void ParseTokens(const TokensList& tokens, const Spectrum& spectrum, Camera& camera);
		static void ParseCamera(TokensIterator& it, TokensIterator& end, Camera& camera);

		static std::unique_ptr<Color> ParseColorType(TokensIterator& it, TokensIterator& end);
		static std::unique_ptr<SpectralColor> ParseSpectralColor(TokensIterator& it, TokensIterator& end);
		static std::unique_ptr<ScalarColor> ParseScalarColor(TokensIterator& it, TokensIterator& end);

		static std::string ParseMaterial(TokensIterator& it, TokensIterator& end, const Spectrum& spectrum);
		static std::unique_ptr<Material> ParseMaterialType(TokensIterator& it, TokensIterator& end, const Spectrum& spectrum);
		static std::shared_ptr<Material> ParseEmbeddedMaterial(TokensIterator& it, TokensIterator& end, const Spectrum& spectrum);
		static std::unique_ptr<LambertianMaterial> ParseLambertianMaterial(TokensIterator& it, TokensIterator& end, const Spectrum& spectrum);
		static std::unique_ptr<PhongLuminaireMaterial> ParsePhongLuminaireMaterial(TokensIterator& it, TokensIterator& end, const Spectrum& spectrum);

		static std::vector<Vec3> ParseVertexPositions(TokensIterator& it, TokensIterator& end);
		static void ParsePolygon(TokensIterator& it, TokensIterator& end, std::vector<unsigned long>& vertices);
		static void ParseComplexPolygon(TokensIterator& it, TokensIterator& end,
			std::vector<unsigned long>& vertices, std::vector<std::vector<unsigned long>>& holes);

		static void ParsePlanarMeshObject(TokensIterator& it, TokensIterator& end, const Spectrum& spectrum);
	};

}

#endif
