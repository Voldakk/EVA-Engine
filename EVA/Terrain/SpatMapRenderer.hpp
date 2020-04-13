#pragma once

#include "EVA.hpp"
#include "EVA/ResourceManagers.hpp"
#include "TerrainLayer.hpp"

namespace EVA
{
	class SplatMapRenderer
	{
		std::shared_ptr<Shader> m_shader;

	public:

		SplatMapRenderer()
		{
			auto paths = std::make_shared<ShaderPaths>();
			paths->compute = "./assets/terrain/shaders/source/splat_map.comp";
			m_shader = std::make_shared<Shader>(paths);
		}

		std::vector<std::shared_ptr<Texture>> Render(int n, std::shared_ptr<Texture> normalMap, std::shared_ptr<Texture> heightMap, std::vector<std::shared_ptr<TerrainLayer>> layers)
		{
			auto splatmap0 = TextureManager::CreateTexStorage(n, n, TextureFormat::RGBA16F, TextureWrapping::ClampToEdge);
			auto splatmap1 = TextureManager::CreateTexStorage(n, n, TextureFormat::RGBA16F, TextureWrapping::ClampToEdge);

			m_shader->Bind();

			m_shader->SetUniform1I("N", n);
			m_shader->BindTexture(normalMap, "normalMap");
			m_shader->BindTexture(heightMap, "heightMap");
			m_shader->BindImageTexture(splatmap0, ImageAccess::WriteOnly, 0);
			m_shader->BindImageTexture(splatmap1, ImageAccess::WriteOnly, 1);

			m_shader->SetUniform1I("numLayers", layers.size());

			for (size_t i = 0; i < layers.size(); i++)
			{
				if (layers[i] == nullptr)
					continue;

				auto var = "layers[" + std::to_string(i) + "].";

				m_shader->SetUniform1F(var + "minHeight", layers[i]->height->min);
				m_shader->SetUniform1F(var + "maxHeight", layers[i]->height->max);

				m_shader->SetUniform1F(var + "minAngle", layers[i]->angle->min);
				m_shader->SetUniform1F(var + "maxAngle", layers[i]->angle->max);
			}

			m_shader->DispatchCompute(n / 16, n / 16);

			m_shader->Unbind();

			return {splatmap0, splatmap1};
		}
	};
}