#include "VertexBuffer.hpp"

#include "EVA/OpenGL.hpp"

namespace EVA
{
	VertexBuffer::VertexBuffer(const void* data, const unsigned int size, const unsigned int usage)
		: m_Usage(usage)
	{
		GLCall(glGenBuffers(1, &m_RendererId));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, m_Usage));
	}

	VertexBuffer::VertexBuffer(const unsigned size, const unsigned int usage)
		: m_Usage(usage)
	{
		GLCall(glGenBuffers(1, &m_RendererId));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, m_Usage));
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererId));
	}

	void VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererId));
	}

	void VertexBuffer::Unbind()
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}

	void VertexBuffer::BufferData(const void* data, const unsigned size) const
	{
		Bind();
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, m_Usage));
	}
}
