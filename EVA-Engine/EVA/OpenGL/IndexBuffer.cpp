#include "IndexBuffer.hpp"

#include "EVA/OpenGL.hpp"

namespace EVA
{
	IndexBuffer::IndexBuffer(const unsigned int* data, const unsigned int count) : m_Count(count)
	{
		GLCall(glGenBuffers(1, &m_RendererId));
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}

	IndexBuffer::~IndexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererId));
	}

	void IndexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererId));
	}

	void IndexBuffer::Unbind()
	{
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
	}
}
