#include "VertexArray.hpp"

#include "EVA/OpenGL.hpp"

namespace EVA
{
	VertexArray::VertexArray() : m_RendererId(0), m_Attributes(0)
	{
		GLCall(glGenVertexArrays(1, &m_RendererId));
	}

	VertexArray::~VertexArray()
	{
		GLCall(glDeleteVertexArrays(1, &m_RendererId));
	}

	void VertexArray::Bind() const
	{
		GLCall(glBindVertexArray(m_RendererId));
	}

	void VertexArray::Unbind()
	{
		GLCall(glBindVertexArray(0));
	}

	void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
	{
		Bind();
		vb.Bind();

		const auto& elements = layout.GetElements();
		unsigned int offset = 0;

		for (auto element : elements)
		{
			GLCall(glEnableVertexAttribArray(m_Attributes));
			GLCall(glVertexAttribPointer(m_Attributes, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);;

			GLCall(glVertexAttribDivisor(m_Attributes, element.divisor));

			m_Attributes++;
		}

		if (layout.patchSize != 0) {
			GLCall(glPatchParameteri(GL_PATCH_VERTICES, layout.patchSize));
		}
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
			GLCall(glEnableVertexAttribArray(attributeCount));
			GLCall(glVertexAttribPointer(attributeCount, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));
			offset += element.count * VertexBufferElement::GetSizeOfType(element.type);;

			GLCall(glVertexAttribDivisor(attributeCount, element.divisor));

			attributeCount++;
		}
	}
}
