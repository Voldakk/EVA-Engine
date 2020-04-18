#include "TerrainMaterial.hpp"

#include "Terrain.hpp"

namespace EVA
{
	inline void TerrainMaterial::SetMaterialUniforms(Scene* scene)
	{
		Material::SetMaterialUniforms(scene);

		if (m_Terrain != nullptr)
		{
			shader->SetUniformMatrix4Fv("worldMatrix", m_Terrain->transform->modelMatrix);

			shader->SetUniform1F("scaleY", m_Terrain->transform->scale.y);
			shader->SetUniform1I("tbnRange", m_Terrain->tbnRange);

			shader->BindTexture(m_Terrain->heightmap, "heightmap");
			shader->BindTexture(m_Terrain->normalmap, "normalmap");
			shader->BindTexture(m_Terrain->splatmaps[0], "splatmap0");
			shader->BindTexture(m_Terrain->splatmaps[1], "splatmap1");

			shader->SetUniform1I("numMaterials", m_Terrain->layers.size());
			for (size_t i = 0; i < m_Terrain->layers.size(); i++)
			{
				if (m_Terrain->layers[i] == nullptr || m_Terrain->layers[i]->material == nullptr)
					continue;

				auto var = "materials[" + std::to_string(i) + "].";

				shader->SetUniform4Fv(var + "tint", m_Terrain->layers[i]->material->tint);
				shader->SetUniform2Fv(var + "tiling", m_Terrain->layers[i]->material->tiling);
				shader->SetUniform1F(var + "heightScale", m_Terrain->layers[i]->material->heightScale);

				shader->BindTexture(m_Terrain->layers[i]->material->GetAlbedoMap(), var + "albedoMap");
				shader->BindTexture(m_Terrain->layers[i]->material->GetMetRouAoMap(), var + "metRouAoMap");
				shader->BindTexture(m_Terrain->layers[i]->material->GetNormalHeightMap(), var + "normalHeightMap");
			}

			if (scene != nullptr && scene->skybox != nullptr)
			{
				shader->BindTexture(scene->skybox->irradianceMap, "irradianceMap");
				shader->BindTexture(scene->skybox->preFilterMap, "prefilterMap");
				shader->BindTexture(scene->skybox->preComputedBRDF, "brdfLUT");
			}
		}
	}

	void TerrainMaterial::SetTerrain(Terrain* terrain)
	{
		m_Terrain = terrain;
	}
}