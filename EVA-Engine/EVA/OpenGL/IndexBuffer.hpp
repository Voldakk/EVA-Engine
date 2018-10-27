#pragma once

namespace EVA
{
	class IndexBuffer
	{
		unsigned int m_RendererId{};
		unsigned int m_Count;

	public:

		IndexBuffer(const unsigned int* data, unsigned int count);
		~IndexBuffer();

		void Bind() const;
		static void Unbind();

		unsigned int GetCount() const { return m_Count; }
	};
}
