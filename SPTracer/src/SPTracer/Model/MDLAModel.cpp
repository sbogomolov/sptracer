#include "../stdafx.h"
#include "../Exception.h"
#include "../Log.h"
#include "../Color/ScalarColor.h"
#include "../Color/SpectralColor.h"
#include "../Color/Spectrum.h"
#include "../Material/LambertianMaterial.h"
#include "../Material/PhongLuminaireMaterial.h"
#include "../Model/Camera.h"
#include "../Object/PlanarMeshObject.h"
#include "MDLAModel.h"

namespace SPTracer
{

	MDLAModel::MDLAModel(std::string fileName, const Spectrum& spectrum, Camera& camera)
	{
		// open file
		std::ifstream file(fileName);
		if (!file)
		{
			std::string s("MDLAModel: Cannot open file: " + fileName);
			Log::Error(s);
			throw Exception(s);
		}

		// read file
		std::ostringstream ss;
		ss << file.rdbuf();

		// get tokens
		auto tokens = GetTokens(ss.str());

		// parse tokens
		ParseTokens(tokens, spectrum, camera);

		// check model
		if (objects_.size() == 0)
		{
			std::string s = "MDLAModel: Model has no objects";
			Log::Error(s);
			throw Exception(s);
		}
	}

	// Splits the text on tokens skipping comments.
	std::vector<std::string> MDLAModel::GetTokens(std::string text)
	{
		TokensList tokens;
		size_t start = -1;
		bool inStrToken = false;
		bool inComment = false;
		bool inAnnotation = false;

		for (size_t i = 0; i < text.length(); i++)
		{
			// continue if inside a string token and current char is not '\"'
			if (inStrToken && (text[i] != '\"'))
			{
				continue;
			}

			// in comment
			if (inComment)
			{
				// comment ended
				if ((text[i] == '\r') || (text[i] == '\n'))
				{
					inComment = false;
				}

				continue;
			}

			// in annotation
			if (inAnnotation)
			{
				// annotation ended
				if (text[i] == ']')
				{
					inAnnotation = false;
				}

				continue;
			}

			// start comment
			if (text[i] == '%' && !inAnnotation)
			{
				inComment = true;
				// NOTE: switch case will store current token
			}

			// start annotation
			if ((text[i] == '[') && !inComment)
			{
				inAnnotation = true;
				// NOTE: switch case will store current token
			}

			switch (text[i])
			{
			case '\r':
			case '\n':
			case ' ':
			case '\t':
			case '%':
			case '[':
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
		}

		return tokens;
	}

	// Checks if iterator is pointing to a token.
	void MDLAModel::CheckToken(TokensIterator& it, TokensIterator& end)
	{
		// check if there is a token
		if (it == end)
		{
			std::string s = "MDLAModel: Unexpected end of file, no more tokens";
			Log::Error(s);
			throw Exception(s);
		}
	}

	// Returns true if iterator is pointing at the "end" token.
	bool MDLAModel::IsEndToken(TokensIterator& it, TokensIterator& end)
	{
		// check if there is a token
		CheckToken(it, end);
		return *it == "end";
	}

	// Checks that iterator is pointing at the "end" token.
	void MDLAModel::MustBeEndToken(TokensIterator& it, TokensIterator& end)
	{
		if (!IsEndToken(it, end))
		{
			std::string s = "MDLAModel: Bad token, expected end, got: " + *it;
			Log::Error(s);
			throw Exception(s);
		}
	}

	// Checks that iterator is pointing at the token with the required keyword.
	void MDLAModel::CheckKeyword(TokensIterator& it, TokensIterator& end, std::string keyword)
	{
		// check if there is a token
		CheckToken(it, end);

		const std::string& type = *it;
		if (type != keyword)
		{
			// wrong keyword
			std::string s = "MDLAModel: Wrong keyword, expected: " + keyword + " got: " + type;
			Log::Error(s);
			throw Exception(s);
		}
	}

	// Gets string from token pointed by iterator.
	std::string MDLAModel::GetString(TokensIterator& it, TokensIterator& end)
	{
		// check if there is a token
		CheckToken(it, end);

		const std::string& token = *it;
		size_t len = token.length();

		// check if this is a string token
		if ((token[0] != '\"') || (token[len - 1] != '\"'))
		{
			std::string s = "MDLAModel: Bad token, expected string, got: " + token;
			Log::Error(s);
			throw Exception(s);
		}

		return token.substr(1, len - 2);
	}

	// Gets floating point number from token pointed by iterator.
	float MDLAModel::GetFloat(TokensIterator& it, TokensIterator& end)
	{
		// check if there is a token
		CheckToken(it, end);

		const std::string& token = *it;
		try
		{
			// try to get float token
			return std::stof(token);
		}
		catch (std::invalid_argument e)
		{
			std::string s = "MDLAModel: Bad token, expected float, got: " + token;
			Log::Error(s);
			throw Exception(s);
		}
	}

	// Gets integer number from token pointed by iterator.
	unsigned long MDLAModel::GetInteger(TokensIterator& it, TokensIterator& end)
	{
		// check if there is a token
		CheckToken(it, end);

		const auto& token = *it;
		try
		{
			// try to get integer token
			return std::stoul(token);
		}
		catch (std::invalid_argument e)
		{
			std::string s = "MDLAModel: Bad token, expected integer, got: " + token;
			Log::Error(s);
			throw Exception(s);
		}
	}

	// Parses the list of tokens to build a model.
	void MDLAModel::ParseTokens(const TokensList& tokens, const Spectrum& spectrum, Camera& camera)
	{
		auto it = tokens.begin();

		// check file type
		if (*it != "mdlflA20")
		{
			std::string s = "MDLAModel: Wrong file type";
			Log::Error(s);
			throw Exception(s);
		}

		auto end = tokens.end();
		while (++it != end)
		{
			const std::string& token = *it;

			if (token == "cmr")
			{
				// camera
				ParseCamera(it, end, camera);
			}
			else if (token == "nmdMtrl")
			{
				// material
				ParseMaterial(it, end, spectrum);
			}
			else if (token == "plnrMsh")
			{
				// planar mesh object
				ParsePlanarMeshObject(it, end, spectrum);
			}
			else
			{
				// unknown keyword
				std::string s = "MDLAModel: Unknown keyword: " + token;
				Log::Error(s);
				throw Exception(s);
			}
		}
	}

	void MDLAModel::ParseCamera(TokensIterator& it, TokensIterator& end, Camera& camera)
	{
		// check keyword
		CheckKeyword(it, end, "cmr");

		camera.name = GetString(++it, end);	// camera name

		camera.p[0] = GetFloat(++it, end);		// center of projection x
		camera.p[1] = GetFloat(++it, end);		// center of projection y
		camera.p[2] = GetFloat(++it, end);		// center of projection z

		camera.n[0] = GetFloat(++it, end);		// image plane normal x
		camera.n[1] = GetFloat(++it, end);		// image plane normal y
		camera.n[2] = GetFloat(++it, end);		// image plane normal z

		camera.up[0] = GetFloat(++it, end);	// up direction x
		camera.up[1] = GetFloat(++it, end);	// up direction y
		camera.up[2] = GetFloat(++it, end);	// up direction z

		camera.f = GetFloat(++it, end);		// distance to image plane
		camera.iw = GetFloat(++it, end);		// image width
		camera.ih = GetFloat(++it, end);		// image height
		camera.icx = GetFloat(++it, end);		// image center x
		camera.icy = GetFloat(++it, end);		// image center y
		camera.t = GetFloat(++it, end);		// time of exposure
		
		MustBeEndToken(++it, end);				// check end token

		// normalize vectors
		camera.n.Normalize();
		camera.up.Normalize();
	}

	std::unique_ptr<Color> MDLAModel::ParseColorType(TokensIterator& it, TokensIterator& end)
	{
		// color type
		CheckToken(it, end);
		const std::string& colorType = *it;

		std::unique_ptr<Color> color;
		if (colorType == "spctrl")
		{
			// spectral color
			color = ParseSpectralColor(it, end);
		}
		else if (colorType == "sclr")
		{
			// scalar color
			color = ParseScalarColor(it, end);
		}
		else
		{
			// unknown color type
			std::string s = "MDLAModel: Unknown color type: " + colorType;
			Log::Error(s);
			throw Exception(s);
		}

		return color;
	}

	std::unique_ptr<SpectralColor> MDLAModel::ParseSpectralColor(TokensIterator& it, TokensIterator& end)
	{
		// check keyword
		CheckKeyword(it, end, "spctrl");

		std::vector<SpectralColor::Amplitude> amplitudes;
		while (!IsEndToken(++it, end))
		{
			// add amplitude
			float l = GetFloat(it, end);
			float a = GetFloat(++it, end);
			amplitudes.push_back(SpectralColor::Amplitude{ l, a });
		}

		return std::make_unique<SpectralColor>(amplitudes);
	}

	std::unique_ptr<ScalarColor> MDLAModel::ParseScalarColor(TokensIterator & it, TokensIterator & end)
	{
		// check keyword
		CheckKeyword(it, end, "sclr");

		float amplitude = GetFloat(++it, end);
		MustBeEndToken(++it, end);
		return std::make_unique<ScalarColor>(amplitude);
	}

	std::string MDLAModel::ParseMaterial(TokensIterator& it, TokensIterator& end, const Spectrum& spectrum)
	{
		// check keyword
		CheckKeyword(it, end, "nmdMtrl");

		// material name
		std::string name = GetString(++it, end);

		// parse material of some type
		auto material = ParseMaterialType(++it, end, spectrum);

		// check end token
		MustBeEndToken(++it, end);

		// add material to model
		this->materials_[name] = std::shared_ptr<Material>(std::move(material));

		return name;
	}

	std::unique_ptr<Material> MDLAModel::ParseMaterialType(TokensIterator& it, TokensIterator& end, const Spectrum& spectrum)
	{
		// material type
		CheckToken(it, end);
		const std::string& type = *it;

		std::unique_ptr<Material> material;
		if (type == "lmbrtn")
		{
			// Lambertian meterial
			material = ParseLambertianMaterial(it, end, spectrum);
		}
		else if (type == "pLmnr")
		{
			// Phong luminaire material
			material = ParsePhongLuminaireMaterial(it, end, spectrum);
		}
		else
		{
			// unknown material type
			std::string s = "MDLAModel: Unknown material type: " + type;
			Log::Error(s);
			throw Exception(s);
		}

		return material;
	}

	std::shared_ptr<Material> MDLAModel::ParseEmbeddedMaterial(TokensIterator& it, TokensIterator& end, const Spectrum& spectrum)
	{
		// material type
		CheckToken(it, end);

		// existing material
		if (*it == "mtrlNm")
		{
			// get material name
			std::string name = GetString(++it, end);

			// get material
			auto m = materials_.find(name);
			
			// check that material exists
			if (m == materials_.end())
			{
				// material does not exist
				std::string s = "MDLAModel: Material with name \"" + name + "\" does not exist";
				Log::Error(s);
				throw Exception(s);
			}

			MustBeEndToken(++it, end);
			return (*m).second;
		}

		// new material
		return std::shared_ptr<Material>(ParseMaterialType(it, end, spectrum));
	}

	std::unique_ptr<LambertianMaterial> MDLAModel::ParseLambertianMaterial(TokensIterator& it, TokensIterator& end, const Spectrum& spectrum)
	{
		// check keyword
		CheckKeyword(it, end, "lmbrtn");

		// parse color
		auto color = ParseColorType(++it, end);
		MustBeEndToken(++it, end);
		return std::make_unique<LambertianMaterial>(std::move(color), spectrum);
	}

	std::unique_ptr<PhongLuminaireMaterial> MDLAModel::ParsePhongLuminaireMaterial(TokensIterator& it, TokensIterator& end, const Spectrum& spectrum)
	{
		// check keyword
		CheckKeyword(it, end, "pLmnr");

		// parse reflective material
		auto reflectiveMaterial = ParseMaterialType(++it, end, spectrum);

		// parse radiant exitance
		auto radiantExitance = ParseColorType(++it, end);

		// parse Phong exponent
		CheckToken(++it, end);
		const std::string& token = *it;
		if (token != "sclr")
		{
			// wrong keywork
			std::string s = "MDLAModel: Wrong keyword for Phong exponent, expected sclr, got " + token;
			Log::Error(s);
			throw Exception(s);
		}

		auto phongExponent = ParseScalarColor(it, end);

		MustBeEndToken(++it, end);
		return std::make_unique<PhongLuminaireMaterial>(
			std::move(reflectiveMaterial), std::move(radiantExitance), phongExponent->GetAmplitude(0.0f), spectrum);
	}

	void MDLAModel::ParseVertexPositions(TokensIterator& it, TokensIterator& end)
	{
		// check keyword
		CheckKeyword(it, end, "vrtxPstn");

		while (!IsEndToken(++it, end))
		{
			float x = GetFloat(it, end);
			float y = GetFloat(++it, end);
			float z = GetFloat(++it, end);

			vertices_.push_back(std::make_shared<Vec3>(x, y, z));
		}
	}

	void MDLAModel::ParsePolygon(TokensIterator& it, TokensIterator& end, std::vector<unsigned long>& vertices)
	{
		// check keyword
		CheckKeyword(it, end, "plygn");

		while (!IsEndToken(++it, end))
		{
			vertices.push_back(GetInteger(it, end));
		}
	}

	void MDLAModel::ParseComplexPolygon(TokensIterator& it, TokensIterator& end,
		std::vector<unsigned long>& outline, std::vector<std::vector<unsigned long>>& holes)
	{
		// check keyword
		CheckKeyword(it, end, "cmplxPly");

		// outline
		ParsePolygon(++it, end, outline);

		// holes
		while (!IsEndToken(++it, end))
		{
			std::vector<unsigned long> hole;
			ParsePolygon(it, end, hole);
			holes.push_back(std::move(hole));
		}
	}

	void MDLAModel::ParsePlanarMeshObject(TokensIterator& it, TokensIterator& end, const Spectrum& spectrum)
	{
		// check keyword
		CheckKeyword(it, end, "plnrMsh");

		// get name
		std::string name = GetString(++it, end);

		// get material
		auto material = ParseEmbeddedMaterial(++it, end, spectrum);
		std::vector<unsigned long> outline;
		std::vector<std::vector<unsigned long>> holes;

		unsigned long verticesStartIndex = static_cast<unsigned long>(vertices_.size());

		while (!IsEndToken(++it, end))
		{
			const std::string& type = *it;
			if (type == "vrtxPstn")
			{
				// get vertices
				ParseVertexPositions(it, end);
			}
			else if (type == "plygn")
			{
				if (outline.size() > 0)
				{
					// duplicate outline polygon
					std::string s = "MDLAModel: Duplicate polygon for planar mesh object";
					Log::Error(s);
					throw Exception(s);
				}

				// polygon
				ParsePolygon(it, end, outline);
			}
			else if (type == "cmplxPly")
			{
				if (outline.size() > 0)
				{
					// duplicate outline polygon
					std::string s = "MDLAModel: Duplicate polygon for planar mesh object";
					Log::Error(s);
					throw Exception(s);
				}

				// complex polygon
				ParseComplexPolygon(it, end, outline, holes);
			}
			else
			{
				// unknown keyword
				std::string s = "MDLAModel: Unknown keyword: " + type;
				Log::Error(s);
				throw Exception(s);
			}
		}

		// check outline
		if (outline.size() < 3)
		{
			// duplicate outline polygon
			std::string s = "MDLAModel: Not enough vertices in outline";
			Log::Error(s);
			throw Exception(s);
		}

		// check holes
		for (const auto& hole : holes)
		{
			if (hole.size() < 3)
			{
				// duplicate outline polygon
				std::string s = "MDLAModel: Not enough vertices in hole";
				Log::Error(s);
				throw Exception(s);
			}
		}

		// shift outline and holes indices
		std::vector<std::shared_ptr<Vec3>> outlinePtr(outline.size());
		std::vector<std::vector<std::shared_ptr<Vec3>>> holesPtr;

		std::transform(outline.begin(), outline.end(), outlinePtr.begin(),
			[&](unsigned long i) { return vertices_[verticesStartIndex + i]; });

		for (auto& hole : holes)
		{
			std::vector<std::shared_ptr<Vec3>> holePtr(hole.size());
			std::transform(hole.begin(), hole.end(), holePtr.begin(),
				[&](unsigned long i) { return vertices_[verticesStartIndex + i]; });
			
			holesPtr.push_back(std::move(holePtr));
		}

		// add object
		objects_.push_back(std::make_shared<PlanarMeshObject>(
			std::move(name),
			std::move(material),
			std::move(outlinePtr),
			std::move(holesPtr)));
	}

}