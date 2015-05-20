#ifndef SPT_OBJ_MODEL_H
#define SPT_OBJ_MODEL_H

#include "../stdafx.h"

namespace SPTracer
{
	class Triangle;
	struct Spectrum;
	class Color;
	class Scene;

	class OBJModel
	{
	public:
		static std::unique_ptr<Scene> Load(std::string fileName, const Spectrum& spectrum);

	private:
		static std::unique_ptr<Scene> scene_;

		OBJModel();

		static void GetKeywordAndValue(std::string line, std::string& keyword, std::string& value);
		static void ParseModelFile(const std::string& fileName, const Spectrum& spectrum);
		static void ParseMaterialsLibFile(const std::string& fileName, const Spectrum& spectrum);
		static void AddTriangles(std::vector<std::shared_ptr<Triangle>> triangles, bool computeNormals);
		static void AddMaterial(
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
