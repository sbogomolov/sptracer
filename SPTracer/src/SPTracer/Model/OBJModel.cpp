#include "../stdafx.h"
#include "../Exception.h"
#include "../Log.h"
#include "../StringUtil.h"
#include "../Util.h"
#include "../Object/Face.h"
#include "../Color/RGBColor.h"
#include "../Color/Spectrum.h"
#include "../Material/LambertianMaterial.h"
#include "../Material/PhongMaterial.h"
#include "../Material/PhongLuminaireMaterial.h"
#include "../Object/Object.h"
#include "../Object/Vertex.h"
#include "OBJModel.h"

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

		// replace tab characters with spaces in value
		StringUtil::Replace(value, "\t", " ");

		// replace all double spaces with spaces
		while (value.find("  ") != value.npos)
		{
			StringUtil::Replace(value, "  ", " ");
		}
	}

	void OBJModel::ParseModelFile(const std::string& fileName, const Spectrum& spectrum)
	{
		// open file
		std::ifstream file(fileName);
		if (!file)
		{
			throw Exception("Cannot open file: " + fileName);
		}

		// vertices arrays
		std::vector<Vec3> vertexCoordinatex;
		std::vector<Vec3> vertexNormals;
		std::vector<Vec3> textureCoordinates;
		std::vector<Vec3> parameterSpaceVertices;

		// current object data
		bool saveObject = false;
		std::string name;
		std::vector<Face> faces;
		std::shared_ptr<Material> material;

		bool computeNormals = true;

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
			else if (keyword == "v")
			{
				// vertex coordinates
				std::vector<float> xyz = StringUtil::GetFloatArray(value, 3, 4, ' ');

				// add new vertex
				vertexCoordinatex.push_back(Vec3(xyz[0], xyz[1], xyz[2]));
			}
			else if (keyword == "vt")
			{
				// texture coordinates
				std::vector<float> uvw = StringUtil::GetFloatArray(value, 2, 3, ' ');

				// add new texture coordinates
				textureCoordinates.push_back(Vec3(
					uvw[0],
					uvw[1],
					uvw.size() > 2 ? uvw[2] : 0.0f));
			}
			else if (keyword == "vn")
			{
				// vertex normal
				std::vector<float> xyz = StringUtil::GetFloatArray(value, 3, ' ');

				// add new vertex normal
				vertexNormals.push_back(Vec3(xyz[0], xyz[1], xyz[2]));
			}
			else if (keyword == "vp")
			{
				// parameter space vertex
				std::vector<float> uvw = StringUtil::GetFloatArray(value, 1, 3, ' ');

				// add new parameter space vertex
				parameterSpaceVertices.push_back(Vec3(
					uvw[0],
					uvw.size() > 1 ? uvw[1] : 0.0f,
					uvw.size() > 2 ? uvw[2] : 0.0f));
			}
			else if (keyword == "o")
			{
				// object name
				name = value;
			}
			else if (keyword == "usemtl")
			{
				// store old object
				if (saveObject)
				{
					AddObject(std::move(name), std::move(material), std::move(faces), computeNormals);
				}

				// reset object data
				faces.clear();
				computeNormals = true;

				// new object
				saveObject = true;
				
				// get material
				const auto it = materials_.find(value);
				if (it == materials_.end())
				{
					std::string msg = "Cannot find material: " + value;
					Log::Error(msg);
					throw Exception(msg);
				}

				// set material
				material = (*it).second;
			}
			else if (keyword == "f")
			{
				// split value on parts
				std::vector<std::string> parts = StringUtil::Split(value, ' ');

				// check count of parts
				if (parts.size() < 3)
				{
					std::string msg = "The face has less than 3 vertices";
					Log::Error(msg);
					throw Exception(msg);
				}

				// vertices
				std::vector<Vertex> vertices;

				// parse parts
				for (auto& part : parts)
				{
					// new vertex
					Vertex v{};

					std::vector<std::string> p = StringUtil::Split(part, '/');
					
					// vertex coordinates
					long index = StringUtil::GetInt(p[0]);
					v.coord = vertexCoordinatex[index >= 0 ? index - 1 : vertexCoordinatex.size() + index];
					
					// texture coordinates
					if ((p.size() > 1) && (p[1].length() > 0))
					{
						index = StringUtil::GetInt(p[1]);
						v.tex = textureCoordinates[index >= 0 ? index - 1 : textureCoordinates.size() + index];
					}

					// vertex normals
					if ((p.size() > 1) && (p[2].length() > 0))
					{
						computeNormals = false;
						index = StringUtil::GetInt(p[2]);
						v.normal = vertexNormals[index >= 0 ? index - 1 : vertexNormals.size() + index];
					}

					// add vertex to array of vertices
					vertices.push_back(std::move(v));
				}

				// check number of vertices
				if (vertices.size() < 3)
				{
					std::string msg = "The face has less than 3 vertices";
					Log::Error(msg);
					throw Exception(msg);
				}

				// add faces
				const Vertex& v1 = vertices[0];
				for (size_t i = 0; i < vertices.size() - 2; i++)
				{
					const Vertex& v2 = vertices[i + 1];
					const Vertex& v3 = vertices[i + 2];

					// new face
					faces.push_back(Face(material, v1, v2, v3));
				}
			}
		}

		// add last object
		AddObject(std::move(name), std::move(material), std::move(faces), computeNormals);
	}

	void OBJModel::ParseMaterialsLibFile(const std::string& fileName, const Spectrum & spectrum)
	{
		// open file
		std::ifstream file(fileName);
		if (!file)
		{
			throw Exception("Cannot open file: " + fileName);
		}

		// material data
		std::string materialName;
		std::unique_ptr<Color> diffuseReflectance;
		std::unique_ptr<Color> specularReflectance;
		std::unique_ptr<Color> radiance;
		float specularExponent;
		float opticalDensity;

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

			// new material
			if (keyword == "newmtl")
			{
				// store old material if any
				if (materialName.length() > 0)
				{
					AddMaterial(
						materialName,
						std::move(diffuseReflectance),
						std::move(specularReflectance),
						std::move(radiance),
						specularExponent,
						opticalDensity,
						spectrum);
				}

				// reset data for new material
				materialName = value;
				diffuseReflectance.reset(nullptr);
				specularReflectance.reset(nullptr);
				radiance.reset(nullptr);
				specularExponent = 0.0f;
				opticalDensity = 0.0f;
			}
			else if (keyword == "kd")
			{
				// diffuse reflectance
				std::vector<float> rgb = StringUtil::GetFloatArray(value, 3, ' ');

				// check that the color is present
				if (std::abs(std::accumulate(rgb.begin(), rgb.end(), 0.0f)) > Util::Eps)
				{
					diffuseReflectance = std::make_unique<RGBColor>(rgb[0], rgb[1], rgb[2]);
				}
			}
			else if (keyword == "ks")
			{
				// specular reflectance
				std::vector<float> rgb = StringUtil::GetFloatArray(value, 3, ' ');
				
				// check that the color is present
				if (std::abs(std::accumulate(rgb.begin(), rgb.end(), 0.0f)) > Util::Eps)
				{
					specularReflectance = std::make_unique<RGBColor>(rgb[0], rgb[1], rgb[2]);
				}
			}
			else if (keyword == "ke")
			{
				// radiance
				std::vector<float> rgb = StringUtil::GetFloatArray(value, 3, ' ');
				
				// check that the color is present
				if (std::abs(std::accumulate(rgb.begin(), rgb.end(), 0.0f)) > Util::Eps)
				{
					radiance = std::make_unique<RGBColor>(rgb[0], rgb[1], rgb[2]);
				}
			}
			else if (keyword == "ns")
			{
				// specular exponent
				specularExponent = StringUtil::GetFloat(value);
			}
			else if (keyword == "ni")
			{
				// specular exponent
				opticalDensity = StringUtil::GetFloat(value);
			}
		}

		// add last material
		if (materialName.length() > 0)
		{
			AddMaterial(
				materialName,
				std::move(diffuseReflectance),
				std::move(specularReflectance),
				std::move(radiance),
				specularExponent,
				opticalDensity,
				spectrum);
		}
	}

	void OBJModel::AddObject(std::string name, std::shared_ptr<Material> material, std::vector<Face> faces, bool computeNormals)
	{
		// create and add object
		auto object = std::make_shared<Object>(std::move(name), std::move(material), std::move(faces), computeNormals);
		objects_.push_back(std::move(object));
	}

	void OBJModel::AddMaterial(
		std::string materialName,
		std::unique_ptr<Color> diffuseReflectance,
		std::unique_ptr<Color> specularReflectance,
		std::unique_ptr<Color> radiance,
		float specularExponent,
		float opticalDensity,
		const Spectrum& spectrum)
	{
		// reflective material
		std::unique_ptr<Material> reflectiveMaterial;

		// check if there is specular reflectance
		if (specularReflectance)
		{
			// Phong material
			reflectiveMaterial = std::make_unique<PhongMaterial>(
				std::move(diffuseReflectance),
				std::move(specularReflectance),
				specularExponent,
				spectrum);
		}
		else if (diffuseReflectance)
		{
			// Lambertian material
			reflectiveMaterial = std::make_unique<LambertianMaterial>(
				std::move(diffuseReflectance),
				spectrum);
		}

		// check of there is any radiance
		if (radiance)
		{
			// Phong Luminaire material
			materials_[materialName] = std::make_shared<PhongLuminaireMaterial>(
				std::move(reflectiveMaterial),
				std::move(radiance),
				1.0f,
				spectrum);
		}
		else
		{
			// Reflective material
			materials_[materialName] = std::shared_ptr<Material>(std::move(reflectiveMaterial));
		}
	}

}
