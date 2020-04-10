#pragma once

#include "EVA.hpp"
#include "EVA/OpenGL.hpp"

namespace EVA
{
	class FrameBuffer
	{
		int m_Width, m_Height;
		unsigned int m_FrameBufferId{};

	public:
		FrameBuffer(int width, int height) : m_Width(width), m_Height(height)
		{
			glGenFramebuffers(1, &m_FrameBufferId);

			glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferId);
		}

		FrameBuffer(int size) : FrameBuffer(size, size) {}

		virtual ~FrameBuffer()
		{
			glDeleteFramebuffers(1, &m_FrameBufferId);
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

		void AttachTexture(std::shared_ptr<Texture> texture, int mip = 0)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, (GLenum)texture->target, texture->id, mip);
		}

		void AttachCubemap(std::shared_ptr<Texture> cubemap, int index, int mip = 0)
		{
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + index, cubemap->id, mip);
		}
	};

	class RenderBuffer
	{
		int m_Width, m_Height;
		unsigned int m_RenderBufferId{};

	public:
		RenderBuffer(int width, int height) : m_Width(width), m_Height(height)
		{
			glGenRenderbuffers(1, &m_RenderBufferId);

			glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferId);
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Width, m_Height);
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferId);
		}

		RenderBuffer(int size) : RenderBuffer(size, size) {}

		virtual ~RenderBuffer()
		{
			glDeleteRenderbuffers(1, &m_RenderBufferId);
		}

		void SetViewport()
		{
			glViewport(0, 0, m_Width, m_Height);
		}
	};
}