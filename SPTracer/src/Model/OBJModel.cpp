#include <algorithm>
#include <fstream>
#include <numeric>
#include <sstream>
#include "OBJModel.h"
#include "../Exception.h"
#include "../Log.h"
#include "../Spectrum.h"
#include "../StringUtil.h"
#include "../Util.h"
#include "Color/RGBColor.h"
#include "Material/LambertianMaterial.h"
#include "Material/PhongMaterial.h"
#include "Material/PhongLuminaireMaterial.h"

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

		// next vertex indices
		size_t iv = 0;
		size_t ivt = 0;
		size_t ivn = 0;
		size_t ivp = 0;

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
				Vec3 v = Vec3{
					xyz[0],		// x
					xyz[1],		// y
					xyz[2]		// z
				};

				// check if vertex already exists
				if (vertices_.size() > iv)
				{
					// update existing vertex
					vertices_[iv]->v = v;
				}
				else
				{
					// add new vertex
					auto vertex = std::make_shared<Vertex>();
					vertex->v = v;
					vertices_.push_back(std::move(vertex));
				}

				// increment next index
				iv++;
			}
			else if (keyword == "vt")
			{
				// vertex texture coordinates
				std::vector<float> uvw = StringUtil::GetFloatArray(value, 2, 3, ' ');
				Vec3 vt = Vec3{
					uvw[0],							// u
					uvw[1],							// v
					uvw.size() > 2 ? uvw[2] : 0.0f	// w
				};

				// check if vertex already exists
				if (vertices_.size() > ivt)
				{
					// update existing vertex
					vertices_[ivt]->vt = vt;
				}
				else
				{
					// add new vertex
					auto vertex = std::make_shared<Vertex>();
					vertex->vt = vt;
					vertices_.push_back(std::move(vertex));
				}

				// increment next index
				ivt++;
			}
			else if (keyword == "vn")
			{
				// vertex normal
				std::vector<float> xyz = StringUtil::GetFloatArray(value, 3, ' ');
				Vec3 vn = Vec3{
					xyz[0],		// x
					xyz[1],		// y
					xyz[2]		// z
				};

				// check if vertex already exists
				if (vertices_.size() > ivn)
				{
					// update existing vertex
					vertices_[ivn]->vn = vn;
				}
				else
				{
					// add new vertex
					auto vertex = std::make_shared<Vertex>();
					vertex->vn = vn;
					vertices_.push_back(std::move(vertex));
				}

				// increment next index
				ivn++;
			}
			else if (keyword == "vp")
			{
				// vertex parameter space coordinates
				std::vector<float> uvw = StringUtil::GetFloatArray(value, 1, 3, ' ');
				Vec3 vp = Vec3{
					uvw[0],								// u
					uvw.size() > 1 ? uvw[1] : 0.0f,		// v
					uvw.size() > 2 ? uvw[2] : 0.0f		// w
				};

				// check if vertex already exists
				if (vertices_.size() > ivp)
				{
					// update existing vertex
					vertices_[ivp]->vp = vp;
				}
				else
				{
					// add new vertex
					auto vertex = std::make_shared<Vertex>();
					vertex->vp = vp;
					vertices_.push_back(std::move(vertex));
				}

				// increment next index
				ivp++;
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
