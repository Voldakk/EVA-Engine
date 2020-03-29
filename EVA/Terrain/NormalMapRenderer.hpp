#pragma once

#include "EVA.hpp"
#include "EVA/ResourceManagers.hpp"

namespace EVA
{
	class NormalMapRenderer
	{
		std::shared_ptr<Shader> m_shader;

	public:

		NormalMapRenderer()
		{
			auto paths = std::make_shared<ShaderPaths>();
			paths->compute = "./assets/standard assets/shaders/source/normal_map.comp";
			m_shader = std::make_shared<Shader>(paths);
		}

		std::shared_ptr<Texture> Render(std::shared_ptr<Texture> heightmap, int n, float strength)
		{
			auto normalmap = TextureManager::CreateTexture(n, n, TextureFormat::RGBA32F, TextureWrapping::ClampToEdge);
			normalmap->type = TextureType::Normal;

			m_shader->Bind();

			m_shader->BindTexture(heightmap, "heightmap", 0);
			m_shader->SetUniform1I("N", n);
			m_shader->SetUniform1F("strength", strength);

			m_shader->BindImageTexture(normalmap, ImageAccess::WriteOnly, 0);

			m_shader->DispatchCompute(n / 16, n / 16);

			m_shader->Unbind();

			return normalmap;
		}
	};
}