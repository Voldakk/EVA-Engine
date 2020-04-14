#include "Quadtree.hpp"

#include "Terrain.hpp"

namespace EVA
{
	Quadtree::Quadtree(Terrain* terrain, glm::vec2 index, Bounds2 bounds)
	{
		m_Terrain = terrain;
		m_Data.lod = 0;
		m_Data.index = index;
		m_Data.bounds = bounds;
	}

	Quadtree::Quadtree(Terrain* terrain, glm::vec2 index, Bounds2 bounds, int lod)
	{
		m_Terrain = terrain;
		m_Data.lod = lod;
		m_Data.index = index;
		m_Data.bounds = bounds;
	}

	void Quadtree::Subdivide()
	{
		if (!m_Leaf)
			return;

		m_Leaf = false;

		for (size_t y = 0; y < 2; y++)
		{
			for (size_t x = 0; x < 2; x++)
			{
				m_Children.push_back(std::make_unique<Quadtree>(
					m_Terrain,
					glm::vec2(x, y),
					Bounds2::MinSize(
						m_Data.bounds.GetMin() + glm::vec2(m_Data.bounds.GetExtents().x * x, m_Data.bounds.GetExtents().y * y),
						m_Data.bounds.GetExtents()),
					m_Data.lod + 1
					));
			}
		}
	}

	void Quadtree::Merge()
	{
		if (m_Leaf)
			return;

		m_Leaf = true;

		children.clear();
	}

	void Quadtree::GetLeafData(std::vector<NodeData>& leafData) const
	{
		if (m_Leaf)
		{
			leafData.push_back(m_Data);
		}
		else
		{
			for (size_t i = 0; i < m_Children.size(); i++)
			{
				m_Children[i]->GetLeafData(leafData);
			}
		}
	}

	void Quadtree::Update(const glm::vec3 cameraPosition)
	{
		if (m_Data.lod >= m_Terrain->lodDistances.size())
			return;

		Bounds2 b = m_Data.bounds;
		b.ScaleXZ(m_Terrain->transform->scale);

		const auto dist = b.DistanceXZ(cameraPosition);

		if (dist < m_Terrain->lodDistances[m_Data.lod])
			Subdivide();
		else if (dist >= m_Terrain->lodDistances[m_Data.lod])
			Merge();

		if (!leaf)
		{
			for (size_t i = 0; i < m_Children.size(); i++)
			{
				m_Children[i]->Update(cameraPosition);
			}
		}
	}

	Quadtree* Quadtree::Find(float x, float y)
	{
		auto center = m_Data.bounds.GetCenter();

		if (center.x == x && center.y == y)
			return this;

		if (m_Leaf)
			return this;

		if (y < center.y && x < center.x)
			return m_Children[0]->Find(x, y);

		else if (y < center.y && x > center.x)
			return m_Children[1]->Find(x, y);

		else if (y > center.y && x < center.x)
			return m_Children[2]->Find(x, y);

		else if (y > center.y && x > center.x)
			return m_Children[3]->Find(x, y);

		return this;
	}

	void Quadtree::CalcTessScale(Quadtree* root)
	{
		if (root == nullptr)
			root = this;

		auto center = m_Data.bounds.GetCenter();
		auto size = m_Data.bounds.GetSize();
		auto width = size.x;
		float margin = 0.000001f;
		Quadtree* t;

		auto GetScale = [this](Quadtree* other)
		{
			if (other->data.lod < m_Data.lod)
				return (float)glm::pow(2, m_Data.lod - other->data.lod);
			else
				return 1.0f;
		};

		// Positive Y (north)
		t = root->Find(center.x, center.y + margin + width / 2.0);
		m_Data.tScalePosY = GetScale(t);

		// Negative Y (south)
		t = root->Find(center.x, center.y - margin - width / 2.0);
		m_Data.tScaleNegY = GetScale(t);

		// Negative X (east)
		t = root->Find(center.x - margin - width / 2.0, center.y);
		m_Data.tScalePosX = GetScale(t);

		// Positive X (west)
		t = root->Find(center.x + margin + width / 2.0, center.y);
		m_Data.tScaleNegX = GetScale(t);


		for (const auto& c : m_Children)
		{
			c->CalcTessScale(root);
		}
	}
}
