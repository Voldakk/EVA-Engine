#include "Quadtree.hpp"

#include "Terrain.hpp"

namespace EVA
{
	Quadtree::Quadtree(Terrain* terrain, const glm::vec3 extents)
	{
		m_Terrain = terrain;
		m_Lod = 0;
		m_Bounds.center = glm::vec3(0.0f);
		m_Bounds.extents = extents;
		m_Bounds.extents.y = 0.01f;
	}

	Quadtree::Quadtree(Terrain* terrain, const Bounds bounds, const int x, const int z, const unsigned int lod)
	{
		m_Terrain = terrain;
		m_Lod = lod;
		m_Bounds.center = bounds.center + glm::vec3(bounds.extents.x * x, 0.0f, bounds.extents.z * z) / 2.0f;
		m_Bounds.extents = bounds.extents / 2.0f;
		m_Bounds.extents.y = 0.01f;
	}

	void Quadtree::Subdivide()
	{
		if (!m_Leaf)
			return;

		m_Leaf = false;

		m_NorthWest = std::make_unique<Quadtree>(m_Terrain, m_Bounds, -1, 1, m_Lod + 1);
		m_NorthEast = std::make_unique<Quadtree>(m_Terrain, m_Bounds, 1, 1, m_Lod + 1);
		m_SouthWest = std::make_unique<Quadtree>(m_Terrain, m_Bounds, -1, -1, m_Lod + 1);
		m_SouthEast = std::make_unique<Quadtree>(m_Terrain, m_Bounds, 1, -1, m_Lod + 1);
	}

	void Quadtree::Merge()
	{
		if (m_Leaf)
			return;

		m_Leaf = true;

		m_NorthWest.reset(nullptr);
		m_NorthEast.reset(nullptr);
		m_SouthWest.reset(nullptr);
		m_SouthEast.reset(nullptr);
	}

	void Quadtree::GetLeafData(std::vector<NodeData>& leafData) const
	{
		if (m_Leaf) 
		{
			leafData.push_back({ m_Bounds, m_Lod });
		}
		else
		{
			m_NorthWest->GetLeafData(leafData);
			m_NorthEast->GetLeafData(leafData);
			m_SouthWest->GetLeafData(leafData);
			m_SouthEast->GetLeafData(leafData);
		}
	}

	void Quadtree::Update(const glm::vec3 cameraPosition)
	{
		if (m_Lod >= m_Terrain->lodDistances.size())
			return;

		const auto dist = m_Bounds.Distance(cameraPosition);

		if (dist < m_Terrain->lodDistances[m_Lod])
			Subdivide();
		else if (dist >= m_Terrain->lodDistances[m_Lod])
			Merge();

		if (!leaf)
		{
			m_NorthWest->Update(cameraPosition);
			m_NorthEast->Update(cameraPosition);
			m_SouthWest->Update(cameraPosition);
			m_SouthEast->Update(cameraPosition);
		}
	}
}