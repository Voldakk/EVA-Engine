#include "QuadtreeNode.hpp"

#include "Quadtree.hpp"
#include <glm/gtc/matrix_transform.hpp>

namespace EVA
{
	QuadtreeNode::QuadtreeNode(Quadtree* quadTree, glm::vec2 index, Bounds2 bounds) : QuadtreeNode(quadTree, index, bounds, 0)
	{
	}

	QuadtreeNode::QuadtreeNode(Quadtree* quadTree, glm::vec2 index, Bounds2 bounds, int lod)
	{
		m_Quadtree = quadTree;
		m_Data.lod = lod;
		m_Data.index = index;
		m_Data.bounds = bounds;
		m_Data.localMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_Data.bounds.GetMin().x, 0.0f, m_Data.bounds.GetMin().y));
		m_Data.localMatrix = glm::scale(m_Data.localMatrix, glm::vec3(m_Data.bounds.GetSize().x, 0.0f, m_Data.bounds.GetSize().y));
	}

	void QuadtreeNode::Subdivide()
	{
		if (!m_Leaf)
			return;

		m_Leaf = false;

		for (size_t y = 0; y < 2; y++)
		{
			for (size_t x = 0; x < 2; x++)
			{
				m_Children.push_back(std::make_unique<QuadtreeNode>(
					m_Quadtree,
					glm::vec2(x, y),
					Bounds2::MinSize(
						m_Data.bounds.GetMin() + glm::vec2(m_Data.bounds.GetExtents().x * x, m_Data.bounds.GetExtents().y * y),
						m_Data.bounds.GetExtents()),
					m_Data.lod + 1
					));
			}
		}
	}

	void QuadtreeNode::Merge()
	{
		if (m_Leaf)
			return;

		m_Leaf = true;

		children.clear();
	}

	void QuadtreeNode::GetLeafData(std::vector<NodeData>& leafData) const
	{
		if (m_Leaf)
		{
			leafData.push_back(m_Data);
		}
		else
		{
			for (size_t i = 0; i < m_Children.size(); i++)
			{
				m_Children[i]->GetLeafData(leafData);
			}
		}
	}

	void QuadtreeNode::Update(const glm::vec2 cameraPosition)
	{
		if (m_Data.lod >= m_Quadtree->lodDistances.size())
			return;

		auto b = m_Data.bounds;
		b.Scale(m_Quadtree->scale);
		const auto dist = b.Distance(cameraPosition);

		if (dist < m_Quadtree->lodDistances[m_Data.lod])
			Subdivide();
		else if (dist >= m_Quadtree->lodDistances[m_Data.lod])
			Merge();

		if (!leaf)
		{
			for (size_t i = 0; i < m_Children.size(); i++)
			{
				m_Children[i]->Update(cameraPosition);
			}
		}
	}

	QuadtreeNode* QuadtreeNode::Find(float x, float y)
	{
		auto center = m_Data.bounds.GetCenter();

		if (center.x == x && center.y == y)
			return this;

		if (m_Leaf)
			return this;

		if (y < center.y && x < center.x)
			return m_Children[0]->Find(x, y);

		else if (y < center.y && x > center.x)
			return m_Children[1]->Find(x, y);

		else if (y > center.y && x < center.x)
			return m_Children[2]->Find(x, y);

		else if (y > center.y && x > center.x)
			return m_Children[3]->Find(x, y);

		return this;
	}

	void QuadtreeNode::CalcTessScale()
	{
		auto center = m_Data.bounds.GetCenter();
		auto size = m_Data.bounds.GetSize();
		auto width = size.x;
		float margin = 0.000001f;
		QuadtreeNode* t;

		auto GetScale = [this](QuadtreeNode* other)
		{
			if (other->data.lod < m_Data.lod)
				return (float)glm::pow(2, m_Data.lod - other->data.lod);
			else
				return 1.0f;
		};

		// Positive Y (north)
		t = m_Quadtree->Find(center.x, center.y + margin + width / 2.0);
		m_Data.tScalePosY = GetScale(t);

		// Negative Y (south)
		t = m_Quadtree->Find(center.x, center.y - margin - width / 2.0);
		m_Data.tScaleNegY = GetScale(t);

		// Negative X (east)
		t = m_Quadtree->Find(center.x - margin - width / 2.0, center.y);
		m_Data.tScalePosX = GetScale(t);

		// Positive X (west)
		t = m_Quadtree->Find(center.x + margin + width / 2.0, center.y);
		m_Data.tScaleNegX = GetScale(t);


		for (const auto& c : m_Children)
		{
			c->CalcTessScale();
		}
	}
}
