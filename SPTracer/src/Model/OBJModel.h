#ifndef SPT_OBJ_MODEL_H
#define SPT_OBJ_MODEL_H

#include <string>
#include "Model.h"

namespace SPTracer
{
	struct Spectrum;

	class OBJModel : public Model
	{
	public:
		OBJModel(std::string fileName, const Spectrum& spectrum);

	private:
		void GetKeywordAndValue(std::string line, std::string& keyword, std::string& value);
		void ParseModelFile(const std::string& fileName, const Spectrum& spectrum);
		void ParseMaterialsLibFile(const std::string& fileName, const Spectrum& spectrum);
	};

}

#endif
