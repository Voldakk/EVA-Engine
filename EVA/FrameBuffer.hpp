#pragma once

#include "EVA.hpp"
#include "EVA/OpenGL.hpp"

namespace EVA
{
	class FrameBufferRenderBuffer
	{
		int m_Width, m_Height;
		unsigned int m_FrameBufferId{};
		unsigned int m_RenderBufferId{};

	public:
		FrameBufferRenderBuffer(int width, int height) : m_Width(width), m_Height(height)
		{
			glGenFramebuffers(1, &m_FrameBufferId);
			glGenRenderbuffers(1, &m_RenderBufferId);

			glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
			glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferId);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Width, m_Height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferId);
		}

		FrameBufferRenderBuffer(int size) : FrameBufferRenderBuffer(size, size) {}

		virtual ~FrameBufferRenderBuffer()
		{
			glDeleteFramebuffers(1, &m_FrameBufferId);
			glDeleteRenderbuffers(1, &m_RenderBufferId);
		}

		void Bind()
		{
			glViewport(0, 0, m_Width, m_Height);
			glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
		}

		void Unbind()
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}

		void AttachTexture(std::shared_ptr<Texture> texture, GLenum target)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, texture->id, 0);
		}

		void AttachCubemap(std::shared_ptr<Texture> cubemap, int index)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, cubemap->id, 0);
		}
	};
}