#include "../stdafx.h"
#include "../Exception.h"
#include "MeshObject.h"

namespace SPTracer
{

	MeshObject::MeshObject(std::string name, std::shared_ptr<Material> material, std::vector<Face> faces, bool computeNormals)
		: Object(std::move(name), std::move(material)), faces_(std::move(faces))
	{
		// compute face normals, edge1 and edge2
		if (computeNormals)
		{
			for (auto& face : faces_)
			{
				const Vec3& v1 = face[0].coord;
				const Vec3& v2 = face[1].coord;
				const Vec3& v3 = face[2].coord;

				const Vec3 n = ComputeNormal(v1, v2, v3);
				for (size_t i = 0; i < 3; i++)
				{
					face[i].normal = n;
				}
			}
		}
	}

	bool MeshObject::Intersect(const Ray& ray, Intersection& intersection) const
	{
		// check intersection with faces
		for (const auto& face : faces_)
		{
			if (IntersectWithTriangle(ray, face, intersection))
			{
				return true;
			}
		}

		return false;
	}

}
