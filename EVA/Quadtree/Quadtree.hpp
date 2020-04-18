#pragma once

#include <vector>
#include <memory>

#include "EVA.hpp"
#include "QuadtreeNode.hpp"

namespace EVA
{
	class Quadtree : public ISerializeable
	{
		std::unique_ptr<QuadtreeNode> m_RootNode;

	public:

		int numLodDistances = 8;
		std::vector<int> lodDistances = std::vector<int>(numLodDistances);
		glm::vec2 scale;

		Quadtree();

		void GetLeafData(std::vector<NodeData>& leafData) const;

		void Update(const glm::vec2 cameraPosition);

		QuadtreeNode* Find(float x, float y);

		virtual void Serialize(DataObject& data) override;
	};
}
