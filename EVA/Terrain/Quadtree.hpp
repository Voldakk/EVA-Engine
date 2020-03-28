#pragma once

#include "../Components/Collider.hpp"
#include "../Bounds.hpp"

namespace EVA
{
	struct NodeData
	{
		Bounds2 bounds;
		int lod;
		glm::vec2 index;
	};

	class Terrain;

	class Quadtree
	{
		NodeData m_Data;
		bool m_Leaf = true;

		Terrain* m_Terrain;

		std::vector<std::unique_ptr<Quadtree>> m_Children;

	public:

		bool& leaf = m_Leaf;
		std::vector<std::unique_ptr<Quadtree>>& children = m_Children;

		explicit Quadtree(Terrain* terrain, glm::vec2 index, Bounds2 bounds);

		explicit Quadtree(Terrain* terrain, glm::vec2 index, Bounds2 bounds, const int lod);

		void Subdivide();

		void Merge();

		void GetLeafData(std::vector<NodeData>& leafData) const;

		void Update(const glm::vec3 cameraPosition);
	};

}