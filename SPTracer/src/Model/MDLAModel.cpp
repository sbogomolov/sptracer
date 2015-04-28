#include <exception>
#include <fstream>
#include <sstream>
#include <vector>
#include "../Exception.h"
#include "../Log.h"
#include "MDLAModel.h"
#include "Color/ScalarColor.h"
#include "Color/SpectralColor.h"
#include "Material/LambertianMaterial.h"
#include "Material/PhongLuminaireMaterial.h"
#include "Object/PlanarMeshObject.h"

namespace SPTracer
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

		// check model
		if (objects_.size() == 0)
		{
			const char* s = "MDLAModel: Model has no objects";
			Log::Error(s);
			throw Exception(s);
		}
	}

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

	void MDLAModel::CheckToken(TokensIterator& it, TokensIterator& end)
	{
		// check if there is a token
		if (it == end)
		{
			auto s = "MDLAModel: Unexpected end of file, no more tokens";
			Log::Error(s);
			throw Exception(s);
		}
	}

	bool MDLAModel::IsEndToken(TokensIterator& it, TokensIterator& end)
	{
		// check if there is a token
		CheckToken(it, end);
		return *it == "end";
	}

	void MDLAModel::MustBeEndToken(TokensIterator& it, TokensIterator& end)
	{
		if (!IsEndToken(it, end))
		{
			std::string s = "MDLAModel: Bad token, expected end, got: " + *it;
			Log::Error(s);
			throw Exception(s.c_str());
		}
	}

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
			throw Exception(s.c_str());
		}
	}

	std::string MDLAModel::GetString(TokensIterator& it, TokensIterator& end)
	{
		// check if there is a token
		CheckToken(it, end);

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

	double MDLAModel::GetDouble(TokensIterator& it, TokensIterator& end)
	{
		// check if there is a token
		CheckToken(it, end);

		const auto& token = *it;
		try
		{
			// try to get double token
			return std::stof(token);
		}
		catch (std::invalid_argument e)
		{
			std::string s = "MDLAModel: Bad token, expected double, got: " + token;
			Log::Error(s);
			throw Exception(s.c_str());
		}
	}

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
			throw Exception(s.c_str());
		}
	}

	void MDLAModel::ParseTokens(const TokensList& tokens)
	{
		auto it = tokens.begin();

		// check file type
		if (*it != "mdlflA20")
		{
			const char* s = "MDLAModel: Wrong file type";
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
				ParseCamera(it, end);
			}
			else if (token == "nmdMtrl")
			{
				// material
				ParseMaterial(it, end);
			}
			else if (token == "plnrMsh")
			{
				// planar mesh object
				ParsePlanarMeshObject(it, end);
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

	void MDLAModel::ParseCamera(TokensIterator& it, TokensIterator& end)
	{
		// check keyword
		CheckKeyword(it, end, "cmr");

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
		
		MustBeEndToken(++it, end);				// check end token

		// normalize vectors
		camera_.n.Normalize();
		camera_.up.Normalize();
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
			throw Exception(s.c_str());
		}

		return color;
	}

	std::unique_ptr<SpectralColor> MDLAModel::ParseSpectralColor(TokensIterator& it, TokensIterator& end)
	{
		// check keyword
		CheckKeyword(it, end, "spctrl");

		auto color = std::make_unique<SpectralColor>();
		while (!IsEndToken(++it, end))
		{
			// add amplitude
			double l = GetDouble(it, end);
			double a = GetDouble(++it, end);
			color->AddAmplitude(l, a);
		}

		color->Init();
		return color;
	}

	std::unique_ptr<ScalarColor> MDLAModel::ParseScalarColor(TokensIterator & it, TokensIterator & end)
	{
		// check keyword
		CheckKeyword(it, end, "sclr");

		double amplitude = GetDouble(++it, end);
		MustBeEndToken(++it, end);
		return std::make_unique<ScalarColor>(amplitude);
	}

	std::string MDLAModel::ParseMaterial(TokensIterator& it, TokensIterator& end)
	{
		// check keyword
		CheckKeyword(it, end, "nmdMtrl");

		// material name
		std::string name = GetString(++it, end);

		// parse material of some type
		auto material = ParseMaterialType(++it, end);

		// check end token
		MustBeEndToken(++it, end);

		// add material to model
		this->materials_[name] = std::shared_ptr<Material>(std::move(material));

		return name;
	}

	std::unique_ptr<Material> MDLAModel::ParseMaterialType(TokensIterator& it, TokensIterator& end)
	{
		// material type
		CheckToken(it, end);
		const std::string& type = *it;

		std::unique_ptr<Material> material;
		if (type == "lmbrtn")
		{
			// Lambertian meterial
			material = ParseLambertianMaterial(it, end);
		}
		else if (type == "pLmnr")
		{
			// Phong luminaire material
			material = ParsePhongLuminaireMaterial(it, end);
		}
		else
		{
			// unknown material type
			std::string s = "MDLAModel: Unknown material type: " + type;
			Log::Error(s);
			throw Exception(s.c_str());
		}

		return material;
	}

	std::shared_ptr<Material> MDLAModel::ParseEmbeddedMaterial(TokensIterator& it, TokensIterator& end)
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
				throw Exception(s.c_str());
			}

			MustBeEndToken(++it, end);
			return (*m).second;
		}

		// new material
		return std::shared_ptr<Material>(ParseMaterialType(it, end));
	}

	std::unique_ptr<LambertianMaterial> MDLAModel::ParseLambertianMaterial(TokensIterator& it, TokensIterator& end)
	{
		// check keyword
		CheckKeyword(it, end, "lmbrtn");

		// parse color
		auto color = ParseColorType(++it, end);
		MustBeEndToken(++it, end);
		return std::make_unique<LambertianMaterial>(std::move(color));
	}

	std::unique_ptr<PhongLuminaireMaterial> MDLAModel::ParsePhongLuminaireMaterial(TokensIterator& it, TokensIterator& end)
	{
		// check keyword
		CheckKeyword(it, end, "pLmnr");

		// parse reflective material
		auto reflectiveMaterial = ParseMaterialType(++it, end);

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
			throw Exception(s.c_str());
		}

		auto phongExponent = ParseScalarColor(it, end);

		MustBeEndToken(++it, end);
		return std::make_unique<PhongLuminaireMaterial>(
			std::move(reflectiveMaterial), std::move(radiantExitance), phongExponent->GetAmplitude(0.0));
	}

	void MDLAModel::ParseVertexPositions(TokensIterator& it, TokensIterator& end, std::vector<Vec3>& vertices)
	{
		// check keyword
		CheckKeyword(it, end, "vrtxPstn");

		while (!IsEndToken(++it, end))
		{
			vertices.push_back(Vec3{
				GetDouble(it, end),		// x
				GetDouble(++it, end),	// y
				GetDouble(++it, end)		// z
			});
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

	void MDLAModel::ParsePlanarMeshObject(TokensIterator& it, TokensIterator& end)
	{
		// check keyword
		CheckKeyword(it, end, "plnrMsh");

		// get name
		std::string name = GetString(++it, end);

		// get material
		auto material = ParseEmbeddedMaterial(++it, end);
		std::vector<Vec3> vertices;
		std::vector<unsigned long> outline;
		std::vector<std::vector<unsigned long>> holes;

		while (!IsEndToken(++it, end))
		{
			const std::string& type = *it;
			if (type == "vrtxPstn")
			{
				// get vertices
				ParseVertexPositions(it, end, vertices);
			}
			else if (type == "plygn")
			{
				if (outline.size() > 0)
				{
					// duplicate outline polygon
					const char* s = "MDLAModel: Duplicate polygon for planar mesh object";
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
					const char* s = "MDLAModel: Duplicate polygon for planar mesh object";
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
				throw Exception(s.c_str());
			}
		}

		// check outline
		if (outline.size() < 3)
		{
			// duplicate outline polygon
			const char* s = "MDLAModel: Not enough vertices in outline";
			Log::Error(s);
			throw Exception(s);
		}

		// check holes
		for (const auto& hole : holes)
		{
			if (hole.size() < 3)
			{
				// duplicate outline polygon
				const char* s = "MDLAModel: Not enough vertices in hole";
				Log::Error(s);
				throw Exception(s);
			}
		}

		// add object
		objects_.push_back(std::make_shared<PlanarMeshObject>(
			std::move(name),
			std::move(material),
			std::move(vertices),
			std::move(outline),
			std::move(holes)));
	}

}