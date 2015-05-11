#ifndef SPT_OBJ_MODEL_H
#define SPT_OBJ_MODEL_H

#include <string>
#include "Model.h"

namespace SPTracer
{
	struct Face;
	struct Spectrum;
	class Color;

	class OBJModel : public Model
	{
	public:
		OBJModel(std::string fileName, const Spectrum& spectrum);

	private:
		void GetKeywordAndValue(std::string line, std::string& keyword, std::string& value);
		void ParseModelFile(const std::string& fileName, const Spectrum& spectrum);
		void ParseMaterialsLibFile(const std::string& fileName, const Spectrum& spectrum);
		void AddObject(std::string name, std::shared_ptr<Material> material, std::vector<Face> faces);
		void AddMaterial(
			std::string materialName,
			std::unique_ptr<Color> diffuseReflectance,
			std::unique_ptr<Color> specularReflectance,
			std::unique_ptr<Color> radiance,
			float specularExponent,
			float opticalDensity,
			const Spectrum& spectrum);
	};

}

#endif
