#pragma once

#include "EVA.hpp"
#include "EVA/ResourceManagers.hpp"

namespace EVA
{
	class SplatMapRenderer
	{
		std::shared_ptr<Shader> m_shader;

	public:

		SplatMapRenderer()
		{
			auto paths = std::make_shared<ShaderPaths>();
			paths->compute = "./assets/standard assets/shaders/source/splat_map.comp";
			m_shader = std::make_shared<Shader>(paths);
		}

		std::shared_ptr<Texture> Render(std::shared_ptr<Texture> normalmap, int n)
		{
			auto splatmap = TextureManager::CreateTexture(n, n, TextureFormat::RGBA16F, TextureWrapping::ClampToEdge);
			splatmap->type = TextureType::Normal;

			m_shader->Bind();

			m_shader->BindTexture(normalmap, "normalmap", 0);
			m_shader->SetUniform1I("N", n);

			m_shader->BindImageTexture(splatmap, ImageAccess::WriteOnly, 0);

			m_shader->DispatchCompute(n / 16, n / 16);

			m_shader->Unbind();

			return splatmap;
		}
	};
}