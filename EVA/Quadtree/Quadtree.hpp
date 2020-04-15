#pragma once

#include <vector>
#include <memory>

#include "QuadtreeNode.hpp"

namespace EVA
{
	class Quadtree
	{
		std::unique_ptr<QuadtreeNode> m_RootNode;

	public:

		std::vector<int> lodDistances;
		glm::vec2 scale;

		Quadtree();

		void GetLeafData(std::vector<NodeData>& leafData) const;

		void Update(const glm::vec2 cameraPosition);

		QuadtreeNode* Find(float x, float y);
	};
}
