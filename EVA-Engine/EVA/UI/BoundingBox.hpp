#pragma once

#include "glm/glm.hpp"

namespace EVA
{
	struct BoundingBox
	{
		glm::vec2 min;
		glm::vec2 max;

		BoundingBox(const float minX, const float maxX, const float minY, const float maxY)
			: min(minX, minY), max(maxX, maxY)
		{
		}

		float Width() const
		{
			return max.x - min.x;
		}

		float Height() const
		{
			return max.y - min.y;
		}

		glm::vec2 Size() const
		{
			return glm::vec2(max.x - min.x, max.y - min.y);
		}

		glm::vec2 Right() const
		{
			return glm::vec2(max.x, min.y + Height() / 2);
		}

		glm::vec2 Left() const
		{
			return glm::vec2(min.x, min.y + Height() / 2);
		}

		glm::vec2 Top() const
		{
			return glm::vec2(min.x + Width() / 2, max.y);
		}

		glm::vec2 Bottom() const
		{
			return glm::vec2(min.x + Width() / 2, min.y);
		}

		glm::vec2 Center() const
		{
			return glm::vec2(min.x + Width() / 2, min.y + Height() / 2);
		}

		bool ContainsPoint(const glm::vec2 point) const
		{
			return point.x > min.x && point.x < max.x && point.y > min.y && point.y < max.y;
		}

		BoundingBox operator+(const float value) const
		{
			return BoundingBox(min.x + value, max.x + value, min.y + value, max.y + value);
		}

		BoundingBox operator-(const float value) const
		{
			return BoundingBox(min.x - value, max.x - value, min.y - value, max.y - value);
		}

		BoundingBox operator+(const glm::vec2 value) const
		{
			return BoundingBox(min.x + value.x, max.x + value.x, min.y + value.y, max.y + value.y);
		}

		BoundingBox operator-(const glm::vec2 value) const
		{
			return BoundingBox(min.x - value.x, max.x - value.x, min.y - value.y, max.y - value.y);
		}
	};
}
