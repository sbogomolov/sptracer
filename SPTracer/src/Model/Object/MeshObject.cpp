#include "../../Exception.h"
#include "MeshObject.h"

namespace SPTracer
{

	MeshObject::MeshObject(std::string name, std::shared_ptr<Material> material, std::vector<Face> faces)
		: Object(std::move(name), std::move(material)), faces_(std::move(faces))
	{
		// compute face normals
		for (auto& face : faces_)
		{
			auto& vert = face.vertices;
			auto& norm = face.normals;

			norm.reserve(vert.size() - 2);
			const Vec3& v1 = *vert[0];
			for (size_t i = 0; i < vert.size() - 2; i++)
			{
				const Vec3& v2 = *vert[i + 1];
				const Vec3& v3 = *vert[i + 2];

				Vec3 n = ComputeNormal(v1, v2, v3);
				norm.push_back(std::move(n));
			}
		}
	}

	bool MeshObject::Intersect(const Ray& ray, Intersection& intersection) const
	{
		// check intersection with faces
		for (const auto& face : faces_)
		{
			const auto& vert = face.vertices;
			const auto& norm = face.normals;

			const Vec3& v1 = *vert[0];
			for (size_t i = 0; i < norm.size(); i++)
			{
				const Vec3& v2 = *vert[i + 1];
				const Vec3& v3 = *vert[i + 2];

				if (IntersectWithTriangle(ray, norm[i], v1, v2, v3, intersection))
				{
					return true;
				}
			}
		}

		return false;
	}

}
