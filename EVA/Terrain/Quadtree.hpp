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

		float tScaleNegX;
		float tScalePosX;
		float tScaleNegY;
		float tScalePosY;
	};

	class Terrain;

	class Quadtree
	{
		bool m_Leaf = true;
		NodeData m_Data;

		Terrain* m_Terrain;

		std::vector<std::unique_ptr<Quadtree>> m_Children;

	public:

		bool& leaf = m_Leaf;
		NodeData& data = m_Data;

		std::vector<std::unique_ptr<Quadtree>>& children = m_Children;

		explicit Quadtree(Terrain* terrain, glm::vec2 index, Bounds2 bounds);

		explicit Quadtree(Terrain* terrain, glm::vec2 index, Bounds2 bounds, const int lod);

		void Subdivide();

		void Merge();

		void GetLeafData(std::vector<NodeData>& leafData) const;

		void Update(const glm::vec3 cameraPosition);

		/**
		* Search for a node in the tree.
		* x, y == the point we are searching for (trying to find the node with an origin closest to that point)
		*/
		Quadtree* Find(float x, float y);

		/**
		* Calculate the tessellation scale factor for a node depending on the neighboring patches.
		*/
		void CalcTessScale(Quadtree* root = nullptr);
	};

}