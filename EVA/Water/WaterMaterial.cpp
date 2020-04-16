#include "WaterMaterial.hpp"

#include "Water.hpp"

void EVA::WaterMaterial::SetMaterialUniforms(Scene* scene)
{
	Material::SetMaterialUniforms(scene);

	if (m_Water != nullptr)
	{
		shader->SetUniform4Fv("material.tint", glm::vec4(0, 0, 1, 0));
		
		shader->SetUniform1F("time", Application::time);
		shader->SetUniform1F("scaleY", m_Water->transform->scale.y);
		shader->SetUniform2Fv("waveSpeed", m_Water->waveSpeed);

		shader->SetUniform1F("scale", m_Water->noise->scale);
		shader->SetUniform1F("persistance", m_Water->noise->persistance);
		shader->SetUniform1F("lacunarity", m_Water->noise->lacunarity);

		shader->SetUniform1F("maxPossibleHeight", m_Water->noise->maxPossibleHeight);

		shader->SetUniform1I("numOctaves", m_Water->noise->octaveOffsets.size());
		for (size_t i = 0; i < m_Water->noise->octaveOffsets.size(); i++)
		{
			shader->SetUniform2Fv("octaveOffsets[" + std::to_string(i) + "]", m_Water->noise->octaveOffsets[i]);
		}

		if (scene != nullptr && scene->skybox != nullptr)
		{
			shader->BindTexture(scene->skybox->irradianceMap, "irradianceMap");
			shader->BindTexture(scene->skybox->preFilterMap, "prefilterMap");
			shader->BindTexture(scene->skybox->preComputedBRDF, "brdfLUT");
		}
	}
}

void EVA::WaterMaterial::SetWater(Water* water)
{
	m_Water = water;
}
