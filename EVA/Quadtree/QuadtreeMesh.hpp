#pragma once

#include <vector>
#include <memory>

#include "glm/glm.hpp"

#include "EVA/OpenGL.hpp"
#include "../Mesh.hpp"

namespace EVA
{
	struct QuadtreeMeshData
	{
		glm::mat4 local;
		float tScaleNegX;
		float tScalePosX;
		float tScaleNegY;
		float tScalePosY;
	};

	class QuadtreeMesh : public EVA::Mesh
	{
		std::unique_ptr<VertexBuffer> m_Db;

	public:

		std::vector<QuadtreeMeshData> data;


		explicit QuadtreeMesh(const std::vector<glm::vec2>& vertices);

		void Draw() const override;
	};

	inline QuadtreeMesh::QuadtreeMesh(const std::vector<glm::vec2>& vertices)
	{
		m_VertexCount = vertices.size();
		m_HasFaceIndices = false;

		m_Va = std::make_unique<VertexArray>();

		{
			m_Vb = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(float) * 2);

			VertexBufferLayout layout;
			layout.Push<float>(2); // Position

			layout.patchSize = vertices.size();

			m_Va->AddBuffer(*m_Vb, layout);
			m_Vb->Unbind();
		}
		{
			m_Db = std::make_unique<VertexBuffer>(0);

			VertexBufferLayout layout;

			layout.Push<float>(4, 1); // Local matrix
			layout.Push<float>(4, 1); // Local matrix
			layout.Push<float>(4, 1); // Local matrix
			layout.Push<float>(4, 1); // Local matrix

			layout.Push<float>(1, 1); // Tesselation scale neg X
			layout.Push<float>(1, 1); // Tesselation scale pos X
			layout.Push<float>(1, 1); // Tesselation scale neg Y
			layout.Push<float>(1, 1); // Tesselation scale pos Y

			layout.patchSize = vertices.size();

			m_Va->AddBuffer(*m_Db, layout);
			m_Db->Unbind();
		}

		m_Va->Unbind();
	}

	inline void QuadtreeMesh::Draw() const
	{
		m_Va->Bind();
		
		m_Db->BufferData(&data[0], data.size() * sizeof(QuadtreeMeshData));

		GLCall(glDrawArraysInstanced(GL_PATCHES, 0, m_VertexCount, data.size()));
		m_Va->Unbind();
	}
}
