#pragma once

#include <vector>
#include <memory>

#include "../Bounds.hpp"

namespace EVA
{
	struct NodeData
	{
		glm::mat4 localMatrix;
		Bounds2 bounds;
		int lod;
		glm::vec2 index;

		float tScaleNegX;
		float tScalePosX;
		float tScaleNegY;
		float tScalePosY;
	};

	class Quadtree;

	class QuadtreeNode
	{
		bool m_Leaf = true;
		NodeData m_Data;

		Quadtree* m_Quadtree;

		std::vector<std::unique_ptr<QuadtreeNode>> m_Children;

	public:

		bool& leaf = m_Leaf;
		NodeData& data = m_Data;

		std::vector<std::unique_ptr<QuadtreeNode>>& children = m_Children;

		explicit QuadtreeNode(Quadtree* quadTree, glm::vec2 index, Bounds2 bounds);

		explicit QuadtreeNode(Quadtree* quadTree, glm::vec2 index, Bounds2 bounds, const int lod);

		void Subdivide();

		void Merge();

		void GetLeafData(std::vector<NodeData>& leafData) const;

		void Update(const glm::vec2 cameraPosition);

		void CalcTessScale();

		QuadtreeNode* Find(float x, float y);
	};

}