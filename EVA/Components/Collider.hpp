#pragma once

#include "EVA.hpp"

namespace EVA
{
	struct Bounds
	{
		glm::vec3 center = glm::vec3(0.0f);
		glm::vec3 extents = glm::vec3(1.0f);

		Bounds() = default;

		Bounds(const glm::vec3 center, const glm::vec3 extents)
		{
			this->center = center;
			this->extents = extents;
		}
		glm::vec3 Min() const
		{
			return center - extents;
		}
		glm::vec3 Max() const
		{
			return center + extents;
		}

		bool Contains(const glm::vec3 point) const
		{
			return
				point.x < center.x + extents.x && point.x > center.x - extents.x &&
				point.y < center.y + extents.y && point.y > center.y - extents.y &&
				point.z < center.z + extents.z && point.z > center.z - extents.z;
		}

		float Distance(const glm::vec3 point) const
		{
			if (Contains((point)))
				return 0.0f;

			const auto min = Min();
			const auto max = Max();

			const auto dx = glm::max(glm::max(min.x - point.x, point.x - max.x), 0.0f);
			const auto dy = glm::max(glm::max(min.y - point.y, point.y - max.y), 0.0f);
			const auto dz = glm::max(glm::max(min.z - point.z, point.z - max.z), 0.0f);

			return glm::sqrt(dx * dx + dy * dy + dz * dz);
		}
	};

	class Collider : public Component
	{

	public:
		
		Bounds bounds;

		~Collider();

		void Awake() override;

		virtual Bounds GetBounds() = 0;

	};

}
