#pragma once

#include "glm/glm.hpp"

struct Bounds
{
private:
	glm::vec3 m_Center = glm::vec3(0.0f);
	glm::vec3 m_Extents = glm::vec3(1.0f);

	glm::vec3 m_Min = glm::vec3(-1.0f);
	glm::vec3 m_Max = glm::vec3(1.0f);

	void RecalculateMinMax()
	{
		m_Min = m_Center - m_Extents;
		m_Max = m_Center + m_Extents;
	}

	void RecalculateCenterExtents()
	{
		m_Center = m_Min + (m_Max - m_Min) / 2.0f;
		m_Extents = (m_Max - m_Min) / 2.0f;
	}

public:

	Bounds() = default;

	Bounds(const glm::vec3 center, const glm::vec3 extents)
	{
		m_Center = m_Center;
		m_Extents = m_Extents;
		RecalculateMinMax();
	}

	bool Contains(const glm::vec3 point) const
	{
		return
			point.x < m_Center.x + m_Extents.x && point.x > m_Center.x - m_Extents.x &&
			point.y < m_Center.y + m_Extents.y && point.y > m_Center.y - m_Extents.y &&
			point.z < m_Center.z + m_Extents.z && point.z > m_Center.z - m_Extents.z;
	}

	float Distance(const glm::vec3 point) const
	{
		if (Contains((point)))
			return 0.0f;

		const auto dx = glm::max(glm::max(m_Min.x - point.x, point.x - m_Max.x), 0.0f);
		const auto dy = glm::max(glm::max(m_Min.y - point.y, point.y - m_Max.y), 0.0f);
		const auto dz = glm::max(glm::max(m_Min.z - point.z, point.z - m_Max.z), 0.0f);

		return glm::sqrt(dx * dx + dy * dy + dz * dz);
	}

	glm::vec3 GetCenter() const { return m_Center; }
	void SetCenter(glm::vec3 center)
	{
		m_Center = center;
		RecalculateMinMax();
	}

	glm::vec3 GetExtents() const { return m_Extents; }
	void SetExtents(glm::vec3 extents)
	{
		m_Extents = extents;
		RecalculateMinMax();
	}

	glm::vec3 GetMin() const { return m_Min; }
	void SetMin(glm::vec3 min)
	{
		m_Min = min;
		RecalculateCenterExtents();
	}

	glm::vec3 GetMax() const { return m_Max; }
	void SetMax(glm::vec3 max)
	{
		m_Max = max;
		RecalculateCenterExtents();
	}
};

struct Bounds2
{
private:
	glm::vec2 m_Center = glm::vec2(0.0f);
	glm::vec2 m_Extents = glm::vec2(1.0f);

	glm::vec2 m_Min = glm::vec2(-1.0f);
	glm::vec2 m_Max = glm::vec2(1.0f);

	void RecalculateMinMax()
	{
		m_Min = m_Center - m_Extents;
		m_Max = m_Center + m_Extents;
	}

	void RecalculateCenterExtents()
	{
		m_Center = m_Min + (m_Max - m_Min) / 2.0f;
		m_Extents = (m_Max - m_Min) / 2.0f;
	}

public:

	Bounds2() = default;

	Bounds2(const glm::vec2 center, const glm::vec2 extents)
	{
		m_Center = center;
		m_Extents = extents;
		RecalculateMinMax();
	}

	bool Contains(const glm::vec2 point) const
	{
		return
			point.x < m_Center.x + m_Extents.x && point.x > m_Center.x - m_Extents.x &&
			point.y < m_Center.y + m_Extents.y && point.y > m_Center.y - m_Extents.y;
	}

	bool ContainsXY(const glm::vec3 point) const
	{
		return
			point.x < m_Center.x + m_Extents.x && point.x > m_Center.x - m_Extents.x &&
			point.y < m_Center.y + m_Extents.y && point.y > m_Center.y - m_Extents.y;
	}

	bool ContainsXZ(const glm::vec3 point) const
	{
		return
			point.x < m_Center.x + m_Extents.x && point.x > m_Center.x - m_Extents.x &&
			point.z < m_Center.y + m_Extents.y && point.z > m_Center.y - m_Extents.y;
	}

	float Distance(const glm::vec2 point) const
	{
		if (Contains((point)))
			return 0.0f;

		const auto dx = glm::max(glm::max(m_Min.x - point.x, point.x - m_Max.x), 0.0f);
		const auto dy = glm::max(glm::max(m_Min.y - point.y, point.y - m_Max.y), 0.0f);

		return glm::sqrt(dx * dx + dy * dy);
	}

	float DistanceXY(const glm::vec3 point) const
	{
		if (ContainsXY((point)))
			return 0.0f;

		const auto dx = glm::max(glm::max(m_Min.x - point.x, point.x - m_Max.x), 0.0f);
		const auto dy = glm::max(glm::max(m_Min.y - point.y, point.y - m_Max.y), 0.0f);

		return glm::sqrt(dx * dx + dy * dy);
	}

	float DistanceXZ(const glm::vec3 point) const
	{
		if (ContainsXZ((point)))
			return 0.0f;

		const auto dx = glm::max(glm::max(m_Min.x - point.x, point.x - m_Max.x), 0.0f);
		const auto dy = glm::max(glm::max(m_Min.y - point.z, point.z - m_Max.y), 0.0f);

		return glm::sqrt(dx * dx + dy * dy);
	}

	glm::vec2 GetCenter() const { return m_Center; }
	void SetCenter(glm::vec2 center)
	{
		m_Center = center;
		RecalculateMinMax();
	}

	glm::vec2 GetExtents() const { return m_Extents; }
	void SetExtents(glm::vec2 extents)
	{
		m_Extents = extents;
		RecalculateMinMax();
	}

	glm::vec2 GetSize() const { return m_Extents * 2.0f; }
	void SetSize(glm::vec2 size)
	{
		m_Extents = size / 2.0f;
		RecalculateMinMax();
	}

	glm::vec2 GetMin() const { return m_Min; }
	void SetMin(glm::vec2 min)
	{
		m_Min = min;
		RecalculateCenterExtents();
	}

	glm::vec2 GetMax() const { return m_Max; }
	void SetMax(glm::vec2 max)
	{
		m_Max = max;
		RecalculateCenterExtents();
	}

	void Scale(const float scale)
	{
		m_Center *= scale;
		m_Extents *= scale;
		RecalculateMinMax();
	}

	void Scale(const glm::vec2 scale)
	{
		m_Center *= scale;
		m_Extents *= scale;
		RecalculateMinMax();
	}

	void ScaleXY(const glm::vec3 scale)
	{
		m_Center *= glm::vec2(scale.x, scale.y);
		m_Extents *= glm::vec2(scale.x, scale.y);
		RecalculateMinMax();
	}

	void ScaleXZ(const glm::vec3 scale)
	{
		m_Center *= glm::vec2(scale.x, scale.z);
		m_Extents *= glm::vec2(scale.x, scale.z);
		RecalculateMinMax();
	}

	static Bounds2 MinMax(const glm::vec2 min, const glm::vec2 max)
	{
		Bounds2 b;
		b.SetMin(min);
		b.SetMax(max);
		return b;
	}

	static Bounds2 MinSize(const glm::vec2 min, const glm::vec2 size)
	{
		Bounds2 b;
		b.SetMin(min);
		b.SetMax(min + size);
		return b;
	}
};

