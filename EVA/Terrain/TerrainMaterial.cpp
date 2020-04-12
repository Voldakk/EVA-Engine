#include "TerrainMaterial.hpp"

#include "Terrain.hpp"

namespace EVA
{
	inline void TerrainMaterial::SetMaterialUniforms(Scene* scene)
	{
		Material::SetMaterialUniforms(scene);

		if (m_Terrain != nullptr)
		{
			shader->SetUniform1F("tessFactor", m_Terrain->tessFactor);
			shader->SetUniform1F("tessSlope", m_Terrain->tessSlope);
			shader->SetUniform1F("tessShift", m_Terrain->tessShift);
			shader->SetUniform1F("scaleY", m_Terrain->transform->scale.y);
			shader->SetUniform1I("tbnRange", m_Terrain->tbnRange);

			shader->SetUniform1I("numMaterials", m_Terrain->materials.size());


			shader->BindTexture(m_Terrain->heightmap, "heightmap");
			shader->BindTexture(m_Terrain->normalmap, "normalmap");
			shader->BindTexture(m_Terrain->splatmap, "splatmap");

			for (size_t i = 0; i < m_Terrain->materials.size(); i++)
			{
				if (m_Terrain->materials[i] == nullptr)
					continue;

				auto var = "materials[" + std::to_string(i) + "].";

				shader->SetUniform4Fv(var + "tint", m_Terrain->materials[i]->tint);
				shader->SetUniform2Fv(var + "tiling", m_Terrain->materials[i]->tiling);
				shader->SetUniform1F(var + "heightScale", m_Terrain->materials[i]->heightScale);

				shader->BindTexture(m_Terrain->materials[i]->GetAlbedoMap(), var + "albedoMap");
				shader->BindTexture(m_Terrain->materials[i]->GetMetRouAoMap(), var + "metRouAoMap");
				shader->BindTexture(m_Terrain->materials[i]->GetNormalHeightMap(), var + "normalHeightMap");
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