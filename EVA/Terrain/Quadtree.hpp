#pragma once

#include "../Components/Collider.hpp"

namespace EVA
{
	struct NodeData
	{
		Bounds bounds;
		unsigned int lod;
	};

	class Terrain;

	class Quadtree
	{
		unsigned int m_Lod = 0;

		bool m_Leaf = true;

		Bounds m_Bounds;

		Terrain* m_Terrain;

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

		explicit Quadtree(Terrain* terrain, const glm::vec3 extents);

		explicit Quadtree(Terrain* terrain, const Bounds bounds, const int x, const int z, const unsigned int lod);

		void Subdivide();

		void Merge();

		void GetLeafData(std::vector<NodeData>& leafData) const;

		void Update(const glm::vec3 cameraPosition);
	};

}