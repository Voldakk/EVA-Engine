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

		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				m_Children.push_back(std::make_unique<Quadtree>(
					m_Terrain,
					glm::vec2(i, j),
					Bounds2::MinSize(
						m_Data.bounds.GetMin() + glm::vec2(m_Data.bounds.GetExtents().x * i, m_Data.bounds.GetExtents().y * j),
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
}
