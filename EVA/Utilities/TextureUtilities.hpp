#pragma once

#include "glm/gtc/matrix_transform.hpp"

#include "EVA.hpp"
#include "../FrameBuffer.hpp"
#include "../TextureManager.hpp"
#include "../ModelManager.hpp"

namespace EVA
{
	class TextureUtilities
	{
		inline static std::shared_ptr<Texture> Convert(std::shared_ptr<Texture> in, std::string inName, int width, int height, std::shared_ptr<Shader> shader)
		{
			FrameBufferRenderBuffer	frameBuffer(width, height);
			auto out = TextureManager::CreateCubeMap(width, height, TextureFormat::RGB16F);
			auto cube = ModelManager::Primitive(ModelManager::PrimitiveType::Cube);

			glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
			glm::mat4 captureViews[] =
			{
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
			   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
			};

			Material material;
			shader->Bind();
			shader->BindTexture(in, inName);
			shader->SetUniformMatrix4Fv("projection", captureProjection);

			frameBuffer.Bind();

			GLCall(glDisable(GL_CULL_FACE));

			for (unsigned int i = 0; i < 6; ++i)
			{
				shader->SetUniformMatrix4Fv("view", captureViews[i]);
				frameBuffer.AttachCubemap(out, i);
				GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));

				cube->GetMesh(0)->Draw();
			}

			GLCall(glEnable(GL_CULL_FACE));

			frameBuffer.Unbind();

			return out;
		}

	public:

		inline static std::shared_ptr<Texture> EquirectangularToCubemap(std::shared_ptr<Texture> hdrTexture, int size = 512)
		{
			auto shader = ShaderManager::LoadShader(ShaderManager::STANDARD_SHADERS_PATH / "equirectangular_to_cubemap.shader");
			return Convert(hdrTexture, "equirectangularMap", size, size, shader);
		}

		inline static std::shared_ptr<Texture> ConvoluteCubemap(std::shared_ptr<Texture> cubemap, int size = 32)
		{
			auto shader = ShaderManager::LoadShader(ShaderManager::STANDARD_SHADERS_PATH / "cubemap_convolution.shader");
			return Convert(cubemap, "environmentMap", size, size, shader);
		}
	};
}