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
		// compute normal
		const Vec3& v1 = vertices_[outline_[0]];
		const Vec3& v2 = vertices_[outline_[1]];
		const Vec3& v3 = vertices_[outline_[2]];
		normal_ = ComputeNormal(v1, v2, v3);

		// compute d parameter for outline
		d_ = (normal_.x * v1.x) + (normal_.y * v1.y) + (normal_.z * v1.z);

		// compute d parameter for holes
		for (size_t j = 0; j < holes_.size(); j++)
		{
			const Vec3& v = vertices_[holes_[j][0]];
			double d = (normal_.x * v.x) + (normal_.y * v.y) + (normal_.z * v.z);
			dh_.push_back(d);
		}
	}

	bool PlanarMeshObject::Intersect(const Ray& ray, Intersection& intersection) const
	{
		// check intersection with holes
		for (size_t j = 0; j < holes_.size(); j++)
		{
			const auto& hole = holes_[j];
			const Vec3& v1 = vertices_[hole[0]];
			double d = dh_[j];

			for (size_t i = 1; i < hole.size() - 1; i++)
			{
				const Vec3& v2 = vertices_[hole[i]];
				const Vec3& v3 = vertices_[hole[i + 1]];
				if (IntersectWithTriangle(ray, v1, v2, v3, normal_, d, intersection))
				{
					return false;
				}
			}
		}

		// check intersection with object
		const Vec3& v1 = vertices_[outline_[0]];
		for (size_t i = 1; i < outline_.size() - 1; i++)
		{
			const Vec3& v2 = vertices_[outline_[i]];
			const Vec3& v3 = vertices_[outline_[i + 1]];
			if (IntersectWithTriangle(ray, v1, v2, v3, normal_, d_, intersection))
			{
				return true;
			}
		}

		return false;
	}

}
