#pragma once

#include <vector>
#include <memory>
#include <string>

#include "glm/glm.hpp"

#include "EVA/OpenGL.hpp"

#include "FileSystem.hpp"

namespace EVA
{

	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texCoords;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	/**
	 * \brief A mesh
	 */
	class Mesh
	{

		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_FaceIndices;

		unsigned int m_InstanceCount;

		std::unique_ptr<VertexArray> m_Va;
		std::unique_ptr<VertexBuffer> m_Vb;
		std::unique_ptr<IndexBuffer> m_Ib;
		std::unique_ptr<VertexBuffer> m_Mb;

		bool m_IsStatic = false;

	public:

		const bool& isStatic = m_IsStatic;
		bool isDirty = true;
		std::string name;

		FS::path path;
		unsigned int index;

		explicit Mesh(std::vector<Vertex> vertices, std::string name = "");

		explicit Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> faceIndices, std::string name = "");

		void Create();

		void Draw() const;

		void DrawInstanced() const;

		void SetMbo(const std::vector<glm::mat4> &models);

		bool HasMbo() const;

		void SetStatic(bool state);
	};

}