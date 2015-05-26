#include "../stdafx.h"
#include "../Util.h"
#include "../Scene/SplitEvent.h"
#include "../Scene/SplitEventType.h"
#include "../Scene/SplitPlane.h"
#include "../Tracer/Intersection.h"
#include "../Tracer/Ray.h"
#include "Triangle.h"

namespace SPTracer
{

	Triangle::Triangle(std::shared_ptr<Material> material)
		: Triangle(std::move(material), Vertex{}, Vertex{}, Vertex{})
	{
	}

	Triangle::Triangle(std::shared_ptr<Material> material, Vertex v1, Vertex v2, Vertex v3)
		: Primitive(std::move(material)), vertices_{ std::move(v1), std::move(v2), std::move(v3) }
	{
		// pre-compute edges for ray-triangle intersection test
		e1_ = vertices_[1].coord - vertices_[0].coord;
		e2_ = vertices_[2].coord - vertices_[0].coord;
	}

	Triangle::~Triangle()
	{
	}

	Vertex& Triangle::operator[](size_t index)
	{
		return vertices_[index];
	}

	const Vertex& Triangle::operator[](size_t index) const
	{
		return vertices_[index];
	}

	const Vec3& Triangle::e1() const
	{
		return e1_;
	}

	const Vec3& Triangle::e2() const
	{
		return e2_;
	}

	void Triangle::ComputeNormals()
	{
		// compute normal vector (using cross product)
		Vec3 normal = e1_.Cross(e2_).Normalize();

		// set normal for vertices
		std::for_each(vertices_.begin(), vertices_.end(), [&normal](Vertex& v) { v.normal = normal; });
	}

	const Box Triangle::GetBox() const
	{
		// compute AABB
		auto result = std::minmax_element(vertices_.begin(), vertices_.end(), [](const Vertex& a, const Vertex& b) {
			return a.coord[0] < b.coord[0];
		});

		float minX = (*result.first).coord[0];
		float maxX = (*result.second).coord[0];

		result = std::minmax_element(vertices_.begin(), vertices_.end(), [](const Vertex& a, const Vertex& b) {
			return a.coord[1] < b.coord[1];
		});

		float minY = (*result.first).coord[1];
		float maxY = (*result.second).coord[1];

		result = std::minmax_element(vertices_.begin(), vertices_.end(), [](const Vertex& a, const Vertex& b) {
			return a.coord[2] < b.coord[2];
		});

		float minZ = (*result.first).coord[2];
		float maxZ = (*result.second).coord[2];

		return Box(Vec3(minX, minY, minZ), Vec3(maxX, maxY, maxZ));
	}

	bool Triangle::Intersect(const Ray& ray, Intersection& intersection) const
	{
		//
		// Moller–Trumbore intersection algorithm
		//

		Vec3 p = ray.direction.Cross(e2_);
		float det = e1_.Dot(p);

		// check determinant
		if (ray.refracted)
		{
			if (det > -Util::Eps)
			{
				// if det is close to 0 - ray lies in plane of triangle
				// if det is positive - ray comes from outside
				return false;
			}
		}
		else
		{
			if (det < Util::Eps)
			{
				// if det is close to 0 - ray lies in plane of triangle
				// if det is negative - ray comes from middle
				return false;
			}
		}

		// invert determinant
		float invDet = 1.0f / det;

		// get first barycentric coordinate
		Vec3 s = ray.origin - vertices_[0].coord;
		float u = invDet * s.Dot(p);

		// check first barycentric coordinate
		if ((u < 0.0f) || (u > 1.0f))
		{
			return false;
		}

		// get second barycentric coordinate
		Vec3 q = s.Cross(e1_);
		float v = invDet * ray.direction.Dot(q);

		// check second and third barycentric coordinates
		if ((v < 0.0f) || ((u + v) > 1.0f))
		{
			return false;
		}

		// at this stage we can compute t to find out where
		// the intersection point is on the line
		float t = invDet * e2_.Dot(q);

		// check intersection
		if (t < Util::Eps)
		{
			// this means that there is a line intersection
			// but not a ray intersection
			return false;
		}

		// ray intersection point
		Vec3 point = ray.origin + t * ray.direction;

		// normal
		intersection.normal = ((1.0f - u - v) * vertices_[0].normal + u * vertices_[1].normal + v * vertices_[2].normal).Normalize();

		// fill the intersection data
		intersection.point = std::move(point);
		// intersection.normal = n;
		intersection.distance = t;

		return true;
	}

	Box Triangle::Clip(const Box& box) const
	{
		// clipped box sizes
		Vec3 clippedMin;
		Vec3 clippedMax;

		// key keyPoints (intersections and keyPoints inside the box)
		std::vector<Vec3> keyPoints;

		// sorted keyPoints of triangle
		std::vector<size_t> left;
		std::vector<size_t> middle;
		std::vector<size_t> right;
		std::vector<size_t> leftAndMiddle;
		std::vector<size_t> rightAndMiddle;

		// each vector can contain not more than 3 keyPoints
		left.reserve(3);
		right.reserve(3);

		// stores information of how many times point was in middle position
		// if point was in middle position 3 times (for X, Y and Z) - than it
		// is onFace the box and should be added to keyPoints list
		std::array<unsigned char, 3> middleCount{ 0, 0, 0 };

		// box max and min
		const Vec3& min = box.min();
		const Vec3& max = box.max();

		// go through all dimensions
		for (unsigned char dimension = 0; dimension < 3; dimension++)
		{
			// clear sorted keyPoints list for next dimension
			left.clear();
			right.clear();
			leftAndMiddle.clear();
			rightAndMiddle.clear();

			// sort points of triangle
			for (size_t i = 0; i < 3; i++)
			{
				const Vec3& v = vertices_[i].coord;

				if (v[dimension] <= min[dimension])
				{
					// point is to the left from the left plane
					left.push_back(i);
					leftAndMiddle.push_back(i);
				}
				else if (v[dimension] >= max[dimension])
				{
					// point is to the right from the right plane
					right.push_back(i);
					rightAndMiddle.push_back(i);
				}
				else
				{
					// point is in between the planes
					leftAndMiddle.push_back(i);
					rightAndMiddle.push_back(i);
					middleCount[i]++;
				}
			}

			// go through all lines that intersect left plane
			for (size_t il : left)
			{
				for (size_t ir : rightAndMiddle)
				{
					// line keyPoints
					const Vec3& a = vertices_[il].coord;
					const Vec3& b = vertices_[ir].coord;

					// distance from line keyPoints to plane
					float da = min[dimension] - a[dimension];
					float db = b[dimension] - min[dimension];

					// intersection factor
					float s = da / (da + db);

					// intersection point
					Vec3 p = a + s * (b - a);

					// check if intersection point is onFace the box's face
					bool onFace = true;
					for (size_t i = 0; i < 3; i++)
					{
						// we check only two other dimesions
						if (i == dimension)
						{
							continue;
						}

						// check that the intersection point is between the bounding planes
						if ((p[i] < min[i]) || (p[i] > max[i]))
						{
							onFace = false;
							break;
						}
					}

					// add intersection point
					if (onFace)
					{
						keyPoints.push_back(std::move(p));
					}
				}
			}

			// go through all lines that intersect right plane
			for (size_t ir : right)
			{
				for (size_t il : leftAndMiddle)
				{
					// line keyPoints
					const Vec3& a = vertices_[il].coord;
					const Vec3& b = vertices_[ir].coord;

					// distance from line keyPoints to plane
					float da = min[dimension] - a[dimension];
					float db = b[dimension] - min[dimension];

					// intersection factor
					float s = da / (da + db);

					// intersection point
					Vec3 p = a + s * (b - a);

					// check if intersection point is onFace the box's face
					bool onFace = true;
					for (unsigned char i = 0; i < 3; i++)
					{
						// we check only two other dimesions
						if (i == dimension)
						{
							continue;
						}

						// check that the intersection point is between the bounding planes
						if ((p[i] < min[i]) || (p[i] > max[i]))
						{
							onFace = false;
							break;
						}
					}

					// add intersection point
					if (onFace)
					{
						keyPoints.push_back(std::move(p));
					}
				}
			}
		}

		// add all keyPoints inside the box to key keyPoints
		for (size_t i = 0; i < 3; i++)
		{
			if (middleCount[i] == 3)
			{
				keyPoints.push_back(vertices_[i].coord);
			}
		}

		// split candidates for each dimension
		std::vector<float> splitCandidates;

		// the maximum possible number of split candidates is the number of key keyPoints
		splitCandidates.reserve(keyPoints.size());

		// find perfect perfect splits for all dimensions
		for (unsigned char dimension = 0; dimension < 3; dimension++)
		{
			// clear split candidates for next dimension
			splitCandidates.clear();

			// go through all key keyPoints and find perfect perfectSplits
			for (const auto& p : keyPoints)
			{
				// add split candidate
				splitCandidates.push_back(p[dimension]);
			}

			// choose perfect perfect splits and set clipped box
			if (splitCandidates.size() == 1)
			{
				// planar clipped box
				clippedMin[dimension] = splitCandidates[0];
				clippedMax[dimension] = splitCandidates[0];
			}
			else if (splitCandidates.size() > 1)
			{
				// use maximum and minimum values of potential split candidates,
				// so that triangle will not be splitted in two parts
				auto result = std::minmax_element(splitCandidates.begin(), splitCandidates.end());
				clippedMin[dimension] = *result.first;
				clippedMax[dimension] = *result.second;
			}
		}

		return Box(std::move(clippedMin), std::move(clippedMax));
	}

}
