#pragma once

#include "../Mesh.hpp"
#include "../Material.hpp"

#include <vector>
#include <memory>

#include "../include/EVA/OpenGL.hpp"

#include "../../dependencies/glm/glm/glm.hpp"

namespace EVA
{

	class TerrainMesh : public EVA::Mesh
	{
	public:

		explicit TerrainMesh(const std::vector<glm::vec3>& vertices);

		void Draw() const override;
		void DrawInstanced(const InstancedMeshData* instancedMeshData) const override;

	};

	inline TerrainMesh::TerrainMesh(const std::vector<glm::vec3>& vertices)
	{
		m_VertexCount = vertices.size();
		m_HasFaceIndices = false;

		m_Va = std::make_unique<VertexArray>();
		m_Vb = std::make_unique<VertexBuffer>(&vertices[0], vertices.size() * sizeof(float) * 3);

		VertexBufferLayout layout;
		layout.Push<float>(3); // Position

		layout.patchSize = vertices.size();

		m_Va->AddBuffer(*m_Vb, layout);

		m_Va->Unbind();
		m_Vb->Unbind();
	}

	inline void TerrainMesh::Draw() const
	{
		m_Va->Bind();
		glDrawArrays(GL_PATCHES, 0, m_VertexCount);
		m_Va->Unbind();
	}

	inline void TerrainMesh::DrawInstanced(const InstancedMeshData* instancedMeshData) const
	{
		m_Va->Bind();

		VertexBufferLayout layout;
		layout.Push<float>(4, 1); // Model matrix
		layout.Push<float>(4, 1); // Model matrix
		layout.Push<float>(4, 1); // Model matrix
		layout.Push<float>(4, 1); // Model matrix

		m_Va->AddTempBuffer(*instancedMeshData->matrixBuffer, layout);

		glDrawArraysInstanced(GL_PATCHES, 0, m_VertexCount, instancedMeshData->instanceCount);
		m_Va->Unbind();
	}
}
