#include "../stdafx.h"
#include "../Material/Material.h"
#include "../Tracer/Intersection.h"
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
		// compute hole faces normals
		for (auto& holeFace : holeFaces_)
		{
			auto& vert = holeFace.vertices;
			auto& norm = holeFace.normals;

			norm.reserve(vert.size() - 2);
			const Vec3& v1 = *vert[0];
			for (size_t i = 0; i < vert.size() - 2; i++)
			{
				const Vec3& v2 = *vert[i + 1];
				const Vec3& v3 = *vert[i + 2];

				// Vertices order is reversed here. In the MDLA file vertices for a hole 
				// are written in a clockwise order as oposed to the vertices of face.
				// Holes are checked for intersection in the same way as face, so their 
				// normals must point in the same direction as face normals. That is why
				// 2nd and 3rd vertices for every triangle are swapped.

				Vec3 n = ComputeNormal(v1, v3, v2);
				norm.push_back(std::move(n));
			}
		}

		// compute face normals
		auto& vert = face_.vertices;
		auto& norm = face_.normals;

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

	bool PlanarMeshObject::Intersect(const Ray& ray, Intersection& intersection) const
	{
		// check intersection with holeFaces face
		for (const auto& holeFace : holeFaces_)
		{
			const auto& vert = holeFace.vertices;
			const auto& norm = holeFace.normals;

			const Vec3& v1 = *vert[0];
			for (size_t i = 0; i < norm.size(); i++)
			{
				const Vec3& v2 = *vert[i + 1];
				const Vec3& v3 = *vert[i + 2];

				// Vertices order is reversed here. In the MDLA file vertices for a hole 
				// are written in a clockwise order as oposed to the vertices of face.
				// Holes are checked for intersection in the same way as face, so their 
				// normals must point in the same direction as face normals. That is why
				// 2nd and 3rd vertices for every triangle are swapped.

				if (IntersectWithTriangle(ray, norm[i], v1, v3, v2, intersection))
				{
					return false;
				}
			}
		}

		// check intersection with face
		const auto& vert = face_.vertices;
		const auto& norm = face_.normals;

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

		return false;
	}

}
