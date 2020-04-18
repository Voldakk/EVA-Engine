#include "Quadtree.hpp"

#include "../ScopeTimer.hpp"

namespace EVA
{
	Quadtree::Quadtree()
	{
		m_RootNode = std::make_unique<QuadtreeNode>(this, glm::vec2(0, 0), Bounds2::MinMax(glm::vec2(0.0f), glm::vec2(1.0f)));
	}

	void Quadtree::GetLeafData(std::vector<NodeData>& leafData) const
	{
		SCOPE_TIMER("Quadtree GetLeafData"); // 1.3 ms
		m_RootNode->GetLeafData(leafData);
	}

	void Quadtree::Update(const glm::vec2 cameraPosition)
	{
		{
			SCOPE_TIMER("Quadtree::Update - Update");
			m_RootNode->Update(cameraPosition);
		}
		{
			SCOPE_TIMER("Quadtree::Update - CalcTessScale");
			m_RootNode->CalcTessScale();
		}
	}

	QuadtreeNode* Quadtree::Find(float x, float y)
	{
		return m_RootNode->Find(x, y);
	}

	void Quadtree::Serialize(DataObject& data)
	{
		bool numLodChanged = data.Serialize("Lod levels", numLodDistances);
		data.Serialize("Lod distances", lodDistances);
		if (numLodChanged)
			lodDistances.resize(numLodDistances);
	}

}