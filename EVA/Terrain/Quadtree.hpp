#pragma once

#include "../Components/Collider.hpp"

namespace EVA
{
	
	class Quadtree
	{
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

		explicit Quadtree(const glm::vec3 extents)
		{
			m_Bounds.center = glm::vec3(0.0f);
			m_Bounds.extents = extents;
			m_Bounds.extents.y = 1.0f;
		}

		Quadtree(const Bounds bounds, const int x, const int z)
		{
			m_Bounds.center = bounds.center + glm::vec3(bounds.extents.x * x, 0.0f, bounds.extents.z * z) / 2.0f;
			m_Bounds.extents = bounds.extents / 2.0f;
			m_Bounds.extents.y = 1.0f;
		}

		void Subdivide()
		{
			m_Leaf = false;

			m_NorthWest = std::make_unique<Quadtree>(m_Bounds, -1,  1);
			m_NorthEast = std::make_unique<Quadtree>(m_Bounds,  1,  1);
			m_SouthWest = std::make_unique<Quadtree>(m_Bounds, -1, -1);
			m_SouthEast = std::make_unique<Quadtree>(m_Bounds,  1, -1);
		}

		void Merge()
		{
			m_Leaf = true;

			m_NorthWest.reset(nullptr);
			m_NorthEast.reset(nullptr);
			m_SouthWest.reset(nullptr);
			m_SouthEast.reset(nullptr);
		}

		void GetLeafBounds(std::vector<Bounds>& boundsList)
		{
			if (m_Leaf)
				boundsList.push_back(m_Bounds);
			else
			{
				m_NorthWest->GetLeafBounds(boundsList);
				m_NorthEast->GetLeafBounds(boundsList);
				m_SouthWest->GetLeafBounds(boundsList);
				m_SouthEast->GetLeafBounds(boundsList);
			}
		}
	};

}
