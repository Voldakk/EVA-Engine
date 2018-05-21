#include "VertexArray.hpp"

#include "EVA/OpenGL.hpp"

namespace EVA
{
	VertexArray::VertexArray() : m_RendererId(0), m_Attributes(0)
	{
		glGenVertexArrays(1, &m_RendererId);
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererId);
	}

	void VertexArray::Bind() const
	{
		glBindVertexArray(m_RendererId);
	}

	void VertexArray::Unbind()
	{
		glBindVertexArray(0);
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		Bind();
		vb.Bind();

		const auto& elements = layout.GetElements();
		unsigned int offset = 0;

		for (auto element : elements)
		{
			glEnableVertexAttribArray(m_Attributes);
			glVertexAttribPointer(m_Attributes, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);;

			glVertexAttribDivisor(m_Attributes, element.divisor);

			m_Attributes++;
		}

		if (layout.patchSize != 0)
			glPatchParameteri(GL_PATCH_VERTICES, layout.patchSize);
	}

	void VertexArray::AddTempBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) const
	{
		Bind();
		vb.Bind();

		const auto& elements = layout.GetElements();
		unsigned int offset = 0;

		auto attributeCount = m_Attributes;

		for (auto element : elements)
		{
			glEnableVertexAttribArray(attributeCount);
			glVertexAttribPointer(attributeCount, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset);
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);;

			glVertexAttribDivisor(attributeCount, element.divisor);

			attributeCount++;
		}
	}
}
