#pragma once

#include "../Components/Collider.hpp"

namespace EVA
{
	struct EVA_API NodeData
	{
		Bounds bounds;
		unsigned int lod;
	};

	class EVA_API Quadtree
	{
		unsigned int m_Lod = 0;
		unsigned int m_MaxLod = 0;

		bool m_Leaf = true;

		Bounds m_Bounds;

		std::unique_ptr<Quadtree> m_NorthWest;
		std::unique_ptr<Quadtree> m_NorthEast;
		std::unique_ptr<Quadtree> m_SouthWest;
		std::unique_ptr<Quadtree> m_SouthEast;

	public:

		bool& leaf = m_Leaf;
		Bounds& bounds = m_Bounds;

		std::unique_ptr<Quadtree>& northWest = m_NorthWest;
		std::unique_ptr<Quadtree>& northEast = m_NorthEast;
		std::unique_ptr<Quadtree>& southWest = m_SouthWest;
		std::unique_ptr<Quadtree>& southEast = m_SouthEast;

		explicit Quadtree(const glm::vec3 extents, const unsigned int maxLod)
		{
			m_Bounds.center = glm::vec3(0.0f);
			m_Bounds.extents = extents;
			m_Bounds.extents.y = 0.01f;

			m_MaxLod = maxLod;
			m_Lod = maxLod;
		}

		Quadtree(const Bounds bounds, const int x, const int z, const unsigned int lod, const unsigned int maxLod)
		{
			m_Bounds.center = bounds.center + glm::vec3(bounds.extents.x * x, 0.0f, bounds.extents.z * z) / 2.0f;
			m_Bounds.extents = bounds.extents / 2.0f;
			m_Bounds.extents.y = 0.01f;

			m_MaxLod = lod;
			m_Lod = lod;
		}

		void Subdivide()
		{
			m_Leaf = false;

			m_NorthWest = std::make_unique<Quadtree>(m_Bounds, -1, 1, m_Lod - 1, m_MaxLod);
			m_NorthEast = std::make_unique<Quadtree>(m_Bounds, 1, 1, m_Lod - 1, m_MaxLod);
			m_SouthWest = std::make_unique<Quadtree>(m_Bounds, -1, -1, m_Lod - 1, m_MaxLod);
			m_SouthEast = std::make_unique<Quadtree>(m_Bounds, 1, -1, m_Lod - 1, m_MaxLod);
		}

		void Merge()
		{
			m_Leaf = true;

			m_NorthWest.reset(nullptr);
			m_NorthEast.reset(nullptr);
			m_SouthWest.reset(nullptr);
			m_SouthEast.reset(nullptr);
		}

		void GetLeafData(std::vector<NodeData>& leafData) const
		{
			if (m_Leaf)
				leafData.push_back({m_Bounds, m_Lod});
			else
			{
				m_NorthWest->GetLeafData(leafData);
				m_NorthEast->GetLeafData(leafData);
				m_SouthWest->GetLeafData(leafData);
				m_SouthEast->GetLeafData(leafData);
			}
		}

		void Update(const glm::vec3 cameraPosition)
		{
			const auto dist = m_Bounds.Distance(cameraPosition);

			const unsigned int targetLod = dist / 10.0f;

			if (targetLod > m_Lod && m_Lod != m_MaxLod)
				Merge();
			else if (targetLod < m_Lod && m_Lod != 0)
				Subdivide();

			if (!leaf)
			{
				m_NorthWest->Update(cameraPosition);
				m_NorthEast->Update(cameraPosition);
				m_SouthWest->Update(cameraPosition);
				m_SouthEast->Update(cameraPosition);
			}
		}
	};
}
