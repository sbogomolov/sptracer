#include "../stdafx.h"
#include "../Exception.h"
#include "MeshObject.h"

namespace SPTracer
{

	MeshObject::MeshObject(std::string name, std::shared_ptr<Material> material, std::vector<Face> faces)
		: Object(std::move(name), std::move(material)), faces_(std::move(faces))
	{
		// compute face normals, e1 and e2
		for (auto& face : faces_)
		{
			face.normals.reserve(face.vertices.size() - 2);
			face.e1.reserve(face.vertices.size() - 2);
			face.e2.reserve(face.vertices.size() - 2);

			const Vec3& v1 = *face.vertices[0];
			for (size_t i = 0; i < face.vertices.size() - 2; i++)
			{
				const Vec3& v2 = *face.vertices[i + 1];
				const Vec3& v3 = *face.vertices[i + 2];

				face.normals.push_back(ComputeNormal(v1, v2, v3));
				face.e1.push_back(v2 - v1);
				face.e2.push_back(v3 - v1);
			}
		}
	}

	bool MeshObject::Intersect(const Ray& ray, Intersection& intersection) const
	{
		// check intersection with faces
		for (const auto& face : faces_)
		{
			const Vec3& v1 = *face.vertices[0];
			for (size_t i = 0; i < face.normals.size(); i++)
			{
				const Vec3& v2 = *face.vertices[i + 1];
				const Vec3& v3 = *face.vertices[i + 2];

				if (IntersectWithTriangle(ray, v1, v2, v3, face.normals[i], face.e1[i], face.e2[i], intersection))
				{
					return true;
				}
			}
		}

		return false;
	}

}
