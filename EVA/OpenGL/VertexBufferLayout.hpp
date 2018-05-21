#pragma once

#include <vector>

#include "GL/glew.h"

namespace EVA
{

	struct VertexBufferElement
	{
		unsigned int type;
		unsigned int count;
		unsigned char normalized;
		unsigned int divisor;

		static unsigned int GetSizeOfType(const unsigned type)
		{
			switch (type)
			{
			case GL_FLOAT: return 4;
			case GL_UNSIGNED_INT: return 4;
			case GL_UNSIGNED_BYTE: return 1;
			default: return 0;
			}
		}
	};

	class VertexBufferLayout
	{

		std::vector<VertexBufferElement> m_Elements;

		unsigned int m_Stride = 0;


	public:

		unsigned int patchSize = 0;
		
		VertexBufferLayout() = default;
		~VertexBufferLayout() = default;

		template<typename T>
		void Push(const unsigned int count, const unsigned int divisor = 0);

		inline unsigned int GetStride() const { return m_Stride; }

		inline std::vector<VertexBufferElement> GetElements() const { return m_Elements; }

	};
}