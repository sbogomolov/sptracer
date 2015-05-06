#include <fstream>
#include <sstream>
#include "OBJModel.h"
#include "../Exception.h"
#include "../Log.h"
#include "../Spectrum.h"
#include "../StringUtil.h"
#include "Material/OBJMaterial.h"

namespace SPTracer
{

	OBJModel::OBJModel(std::string fileName, const Spectrum& spectrum)
	{
		try
		{
			ParseModelFile(fileName, spectrum);
		}
		catch (Exception e)
		{
			std::string msg = "OBJModel: " + std::string(e.what());
			Log::Error(msg);
			throw Exception(msg);
		}
	}

	void OBJModel::GetKeywordAndValue(std::string line, std::string& keyword, std::string& value)
	{
		keyword = "";
		value = "";

		// skip all white spaces and tab characters
		StringUtil::TrimBegin(line, " \t");

		// trim comment if any
		size_t pos = line.find('#');
		if (pos != line.npos)
		{
			line.erase(pos);
		}

		// check for empty line
		if (line.length() == 0)
		{
			return;
		}
		
		// get keyword and value
		pos = line.find_first_of(" \t");
		if (pos == line.npos)
		{
			throw Exception("Error in line: " + line);
		}

		// get lowercase keyword
		keyword = StringUtil::ToLower(line.substr(0, pos));

		// get value and trim all white spaces and tab characters
		value = StringUtil::Trim(line.substr(pos + 1), " \t");
	}

	void OBJModel::ParseModelFile(const std::string& fileName, const Spectrum& spectrum)
	{
		// open file
		std::ifstream file(fileName);
		if (!file)
		{
			throw Exception("Cannot open file: " + fileName);
		}

		// read model file
		std::string line;
		while (std::getline(file, line))
		{
			std::string keyword;
			std::string value;
			GetKeywordAndValue(line, keyword, value);

			// skip empty line
			if (keyword.length() == 0)
			{
				continue;
			}

			if (keyword == "mtllib")
			{
				// materials library
				ParseMaterialsLibFile(value, spectrum);
			}

		}
	}

	void OBJModel::ParseMaterialsLibFile(const std::string& fileName, const Spectrum & spectrum)
	{
		// open file
		std::ifstream file(fileName);
		if (!file)
		{
			throw Exception("Cannot open file: " + fileName);
		}

		// read model file
		std::string line;
		std::string materialName;
		std::shared_ptr<OBJMaterial> material;
		while (std::getline(file, line))
		{
			std::string keyword;
			std::string value;
			GetKeywordAndValue(line, keyword, value);

			// skip empty line
			if (keyword.length() == 0)
			{
				continue;
			}

			if (keyword == "newmtl")
			{
				// store old material if any
				if (material.get() != nullptr)
				{
					materials_[materialName] = material;
				}

				// new material
				materialName = value;
				// material = std::make_shared<OBJMaterial>();
			}

		}
	}

}
