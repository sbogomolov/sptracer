#include "../stdafx.h"
#include "../Material/Material.h"
#include "PlanarMeshObject.h"

namespace SPTracer
{

	PlanarMeshObject::PlanarMeshObject(
		std::string name,
		std::shared_ptr<Material> material,
		std::vector<Face> faces,
		std::vector<Face> holeFaces)
		: Object(std::move(name), std::move(material)),
		faces_(std::move(faces)),
		holeFaces_(std::move(holeFaces))
	{
		// compute hole faces normals, edge1 and edge2
		for (auto& face : holeFaces_)
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

		// compute face normals, edge1 and edge2
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

	bool PlanarMeshObject::Intersect(const Ray& ray, Intersection& intersection) const
	{
		// check intersection with holeFaces face
		for (const auto& face : holeFaces_)
		{
			if (IntersectWithTriangle(ray, face, intersection))
			{
				return false;
			}
		}

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
