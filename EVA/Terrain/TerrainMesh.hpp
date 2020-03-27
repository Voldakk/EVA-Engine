#pragma once

#include "../Mesh.hpp"
#include "../Material.hpp"

#include <vector>
#include <memory>

#include "EVA/OpenGL.hpp"

#include "glm/glm.hpp"

namespace EVA
{
	struct TerrainMeshData
	{
		glm::mat4 model;
		int lod;
		glm::vec2 index;
	};

	class TerrainMesh : public EVA::Mesh
	{
		std::unique_ptr<VertexBuffer> m_Data;

	public:

		explicit TerrainMesh(const std::vector<glm::vec2>& vertices);

		void DrawTerrain(const std::vector<TerrainMeshData>& data);
	};

	inline TerrainMesh::TerrainMesh(const std::vector<glm::vec2>& vertices)
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
			m_Data = std::make_unique<VertexBuffer>(0);

			VertexBufferLayout layout;
			layout.Push<float>(4, 1); // Model matrix
			layout.Push<float>(4, 1); // Model matrix
			layout.Push<float>(4, 1); // Model matrix
			layout.Push<float>(4, 1); // Model matrix
			layout.Push<int>(1, 1);   // Lod
			layout.Push<float>(2, 1); // Index

			layout.patchSize = vertices.size();

			m_Va->AddBuffer(*m_Data, layout);
			m_Data->Unbind();
		}

		m_Va->Unbind();
	}

	inline void TerrainMesh::DrawTerrain(const std::vector<TerrainMeshData>& data)
	{
		m_Va->Bind();
		
		m_Data->BufferData(&data[0], data.size() * sizeof(TerrainMeshData));

		GLCall(glDrawArraysInstanced(GL_PATCHES, 0, m_VertexCount, data.size()));
		m_Va->Unbind();
	}
}
