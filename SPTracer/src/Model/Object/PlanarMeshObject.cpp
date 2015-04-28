#include "../../Intersection.h"
#include "../Material/Material.h"
#include "PlanarMeshObject.h"

namespace SPTracer
{

	PlanarMeshObject::PlanarMeshObject(
		std::string name,
		std::shared_ptr<Material> material,
		std::vector<Vec3> vertices,
		std::vector<unsigned long> outline,
		std::vector<std::vector<unsigned long>> holes)
		: Object(std::move(name), std::move(material)),
		  vertices_(std::move(vertices)),
		  outline_(std::move(outline)),
		  holes_(std::move(holes))
	{
		// compute holes normals and d
		holesNormals_.resize(holes_.size());

		for (size_t j = 0; j < holes_.size(); j++)
		{
			const auto& hole = holes_[j];
			auto& holeNormals = holesNormals_[j];
			
			holeNormals.reserve(hole.size() - 2);

			const Vec3& v1 = vertices_[hole[0]];
			for (size_t i = 0; i < hole.size() - 2; i++)
			{
				const Vec3& v2 = vertices_[hole[i + 1]];
				const Vec3& v3 = vertices_[hole[i + 2]];

				// Vertices order is reversed here. In the MDLA file vertices for a hole 
				// are written in a clockwise order as oposed to the vertices of outline.
				// Holes are checked for intersection in the same way as outline, so their 
				// normals must point in the same direction as outline normals. That is why
				// 2nd and 3rd vertices for every triangle are swapped.

				Vec3 n = ComputeNormal(v1, v3, v2);
				holeNormals.push_back(std::move(n));
			}
		}
		
		// compute outline normals and d
		outlineNormals_.reserve(outline_.size() - 2);

		const Vec3& v1 = vertices_[outline_[0]];
		for (size_t i = 0; i < outline_.size() - 2; i++)
		{
			const Vec3& v2 = vertices_[outline_[i + 1]];
			const Vec3& v3 = vertices_[outline_[i + 2]];
			
			Vec3 n = ComputeNormal(v1, v2, v3);
			outlineNormals_.push_back(std::move(n));
		}
	}

	bool PlanarMeshObject::Intersect(const Ray& ray, Intersection& intersection) const
	{
		// check intersection with holes
		for (size_t j = 0; j < holes_.size(); j++)
		{
			const auto& hole = holes_[j];
			const auto& holeNormals = holesNormals_[j];

			const Vec3& v1 = vertices_[hole[0]];
			for (size_t i = 0; i < hole.size() - 2; i++)
			{
				const Vec3& v2 = vertices_[hole[i + 1]];
				const Vec3& v3 = vertices_[hole[i + 2]];

				// Vertices order is reversed here. In the MDLA file vertices for a hole 
				// are written in a clockwise order as oposed to the vertices of outline.
				// Holes are checked for intersection in the same way as outline, so their 
				// normals must point in the same direction as outline normals. That is why
				// 2nd and 3rd vertices for every triangle are swapped.

				if (IntersectWithTriangle(ray, holeNormals[i], v1, v3, v2, intersection))
				{
					return false;
				}
			}
		}

		// check intersection with object
		const Vec3& v1 = vertices_[outline_[0]];
		for (size_t i = 0; i < outline_.size() - 2; i++)
		{
			const Vec3& v2 = vertices_[outline_[i + 1]];
			const Vec3& v3 = vertices_[outline_[i + 2]];
			if (IntersectWithTriangle(ray, outlineNormals_[i], v1, v2, v3, intersection))
			{
				return true;
			}
		}

		return false;
	}

}
