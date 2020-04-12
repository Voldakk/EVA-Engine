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
		inline static unsigned int quadVAO = 0;
		inline static unsigned int quadVBO;
		inline static void renderQuad()
		{
			if (quadVAO == 0)
			{
				float quadVertices[] = {
					// positions        // texture Coords
					-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
					-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
					 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
					 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
				};
				// setup plane VAO
				glGenVertexArrays(1, &quadVAO);
				glGenBuffers(1, &quadVBO);
				glBindVertexArray(quadVAO);
				glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
				glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
				glEnableVertexAttribArray(0);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
				glEnableVertexAttribArray(1);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
			}
			glBindVertexArray(quadVAO);
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
			glBindVertexArray(0);
		}

		inline static void Convert(std::shared_ptr<Texture> in, std::string inName, std::shared_ptr<Texture> out, std::shared_ptr<Shader> shader)
		{
			FrameBuffer frameBuffer(out->width, out->height);
			RenderBuffer renderBuffer(out->width, out->height);
			
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

			frameBuffer.Unbind();
		}

	public:

		inline static std::shared_ptr<Texture> EquirectangularToCubemap(std::shared_ptr<Texture> hdrTexture, int size = 512)
		{
			auto shader = ShaderManager::LoadShader(ShaderManager::STANDARD_SHADERS_PATH / "equirectangular_to_cubemap.shader");
			auto out = TextureManager::CreateCubeMap(size, size, TextureFormat::RGB16F, TextureWrapping::ClampToEdge, TextureMinFilter::LinearMipmapLinear);

			Convert(hdrTexture, "equirectangularMap", out, shader);

			// Generate mipmaps
			GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, out->id));
			GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));

			return out;
		}

		inline static std::shared_ptr<Texture> ConvoluteCubemap(std::shared_ptr<Texture> cubemap, int size = 32)
		{
			auto shader = ShaderManager::LoadShader(ShaderManager::STANDARD_SHADERS_PATH / "cubemap_convolution.shader");
			auto out = TextureManager::CreateCubeMap(size, size, TextureFormat::RGB16F);
			
			Convert(cubemap, "environmentMap", out, shader);

			return out;
		}

		inline static std::shared_ptr<Texture> PreFilterEnviromentMap(std::shared_ptr<Texture> hdrTexture, int size = 128)
		{
			auto shader = ShaderManager::LoadShader(ShaderManager::STANDARD_SHADERS_PATH / "pre_filter_map.shader");
			auto out = TextureManager::CreateCubeMap(size, size, TextureFormat::RGB16F, TextureWrapping::ClampToEdge, TextureMinFilter::LinearMipmapLinear);
			GLCall(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));

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

			shader->Bind();
			shader->BindTexture(hdrTexture, "environmentMap");
			shader->SetUniformMatrix4Fv("projection", captureProjection);
			shader->SetUniform1F("resolution", hdrTexture->width);

			FrameBuffer frameBuffer(out->width, out->height);
			frameBuffer.Bind();
			
			GLCall(glDisable(GL_CULL_FACE));

			unsigned int maxMipLevels = 5;
			for (unsigned int mip = 0; mip < maxMipLevels; ++mip)
			{
				// Reisze framebuffer according to mip-level size.
				unsigned int mipWidth = 128 * std::pow(0.5, mip);
				unsigned int mipHeight = 128 * std::pow(0.5, mip);

				RenderBuffer renderBuffer(mipWidth, mipHeight);
				renderBuffer.SetViewport();

				float roughness = (float)mip / (float)(maxMipLevels - 1);
				shader->SetUniform1F("roughness", roughness);
				for (unsigned int i = 0; i < 6; ++i)
				{
					shader->SetUniformMatrix4Fv("view", captureViews[i]);
					frameBuffer.AttachCubemap(out, i, mip);

					glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
					cube->GetMesh(0)->Draw();
				}
			}

			frameBuffer.Unbind();

			return out;
		}

		inline static std::shared_ptr<Texture> PreComputeBRDF(int size = 512)
		{
			/*auto shader = ShaderManager::LoadShader(ShaderManager::STANDARD_SHADERS_PATH / "pre_compute_brdf.shader");
			auto out = TextureManager::CreateTexture(size, size, TextureFormat::RG16F, TextureWrapping::ClampToEdge);

			auto plane = ModelManager::Primitive(ModelManager::PrimitiveType::Plane);


			FrameBuffer frameBuffer(out->width, out->height);
			RenderBuffer renderBuffer(out->width, out->height);

			frameBuffer.Bind();
			frameBuffer.AttachTexture(out);
			renderBuffer.SetViewport();

			shader->Bind();

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderQuad();

			frameBuffer.Unbind();
			
			return out;*/

			return TextureManager::LoadTexture("./assets/standard assets/textures/ibl_brdf_lut.png", TextureWrapping::ClampToEdge);
		}
	};
}