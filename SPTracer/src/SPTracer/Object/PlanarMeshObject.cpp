#include "../stdafx.h"
#include "../Material/Material.h"
#include "PlanarMeshObject.h"

namespace SPTracer
{

	PlanarMeshObject::PlanarMeshObject(
		std::string name,
		std::shared_ptr<Material> material,
		Face face,
		std::vector<Face> holeFaces)
		: Object(std::move(name), std::move(material)),
		face_(std::move(face)),
		holeFaces_(std::move(holeFaces))
	{
		// compute hole faces normals, edge1 and edge2
		for (auto& holeFace : holeFaces_)
		{
			holeFace.normals.reserve(holeFace.vertices.size() - 2);
			holeFace.edge1.reserve(holeFace.vertices.size() - 2);
			holeFace.edge2.reserve(holeFace.vertices.size() - 2);

			const Vec3& v1 = *holeFace.vertices[0];
			for (size_t i = 0; i < holeFace.vertices.size() - 2; i++)
			{
				// Vertices order is reversed here. In the MDLA file vertices for a hole 
				// are written in a clockwise order as oposed to the vertices of face.
				// Holes are checked for intersection in the same way as face, so their 
				// normals must point in the same direction as face normals. That is why
				// 2nd and 3rd vertices for every triangle are swapped.

				const Vec3& v2 = *holeFace.vertices[i + 2];
				const Vec3& v3 = *holeFace.vertices[i + 1];

				holeFace.normals.push_back(ComputeNormal(v1, v2, v3));
				holeFace.edge1.push_back(v2 - v1);
				holeFace.edge2.push_back(v3 - v1);
			}
		}

		// compute face normals, edge1 and edge2
		face_.normals.reserve(face_.vertices.size() - 2);
		face_.edge1.reserve(face_.vertices.size() - 2);
		face_.edge2.reserve(face_.vertices.size() - 2);

		const Vec3& v1 = *face_.vertices[0];
		for (size_t i = 0; i < face_.vertices.size() - 2; i++)
		{
			const Vec3& v2 = *face_.vertices[i + 1];
			const Vec3& v3 = *face_.vertices[i + 2];

			face_.normals.push_back(ComputeNormal(v1, v2, v3));
			face_.edge1.push_back(v2 - v1);
			face_.edge2.push_back(v3 - v1);
		}
	}

	bool PlanarMeshObject::Intersect(const Ray& ray, Intersection& intersection) const
	{
		// check intersection with holeFaces face
		for (const auto& holeFace : holeFaces_)
		{
			const Vec3& v1 = *holeFace.vertices[0];
			for (size_t i = 0; i < holeFace.normals.size(); i++)
			{
				// Vertices order is reversed here. In the MDLA file vertices for a hole 
				// are written in a clockwise order as oposed to the vertices of face.
				// Holes are checked for intersection in the same way as face, so their 
				// normals must point in the same direction as face normals. That is why
				// 2nd and 3rd vertices for every triangle are swapped.

				const Vec3& v2 = *holeFace.vertices[i + 2];
				const Vec3& v3 = *holeFace.vertices[i + 1];

				if (IntersectWithTriangle(ray, v1, v2, v3, holeFace.normals[i], holeFace.edge1[i], holeFace.edge2[i], intersection))
				{
					return false;
				}
			}
		}

		// check intersection with face
		const Vec3& v1 = *face_.vertices[0];
		for (size_t i = 0; i < face_.normals.size(); i++)
		{
			const Vec3& v2 = *face_.vertices[i + 1];
			const Vec3& v3 = *face_.vertices[i + 2];

			if (IntersectWithTriangle(ray, v1, v2, v3, face_.normals[i], face_.edge1[i], face_.edge2[i], intersection))
			{
				return true;
			}
		}

		return false;
	}

}
