#pragma once

#include "GL/glew.h"

namespace EVA
{
	class VertexBuffer
	{
		unsigned int m_RendererId{};
		unsigned int m_Usage;

	public:

		VertexBuffer(const void* data, unsigned int size, unsigned int usage = GL_STATIC_DRAW);
		explicit VertexBuffer(unsigned int size, unsigned int usage = GL_STATIC_DRAW);
		~VertexBuffer();

		void Bind() const;
		static void Unbind();

		void BufferData(const void* data, unsigned int size) const;
	};
}
