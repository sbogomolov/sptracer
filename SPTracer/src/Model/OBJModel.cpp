#include <fstream>
#include <sstream>
#include "OBJModel.h"
#include "../Exception.h"
#include "../Log.h"
#include "../Spectrum.h"
#include "../StringUtil.h"

namespace SPTracer
{

	OBJModel::OBJModel(std::string fileName, const Spectrum& spectrum)
	{
		// open file
		std::ifstream file(fileName);
		if (!file)
		{
			std::string s("OBJModel: Cannot open file: " + fileName);
			Log::Error(s);
			throw Exception(s);
		}

		try
		{
			// read file
			std::string line;
			while (std::getline(file, line))
			{
				// store the original line
				std::string originalLine(line);

				// skip all white spaces and tab characters
				StringUtil::TrimBegin(line, " \t");

				// trim comment if any
				size_t pos = line.find('#');
				if (pos != line.npos)
				{
					line.erase(pos);
				}

				// skip empty line
				if (line.length() == 0)
				{
					continue;
				}
			}
		}
		catch (Exception e)
		{
			std::string msg = "OBJModel: " + std::string(e.what());
			Log::Error(msg);
			throw Exception(msg);
		}
	}

}
