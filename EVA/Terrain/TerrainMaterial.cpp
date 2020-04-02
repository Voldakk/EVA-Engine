#include "TerrainMaterial.hpp"

#include "Terrain.hpp"

namespace EVA
{
	inline void TerrainMaterial::SetObjectUniforms(Transform* transform)
	{
		Material::SetObjectUniforms(transform);

		if (m_Terrain != nullptr)
		{
			shader->SetUniform1F("tessFactor", m_Terrain->tessFactor);
			shader->SetUniform1F("tessSlope", m_Terrain->tessSlope);
			shader->SetUniform1F("tessShift", m_Terrain->tessShift);
			shader->SetUniform1F("scaleY", m_Terrain->transform->scale.y);
			shader->SetUniform1I("tbnRange", m_Terrain->tbnRange);

			shader->BindTexture(m_Terrain->heightmap, "heightmap");
			shader->BindTexture(m_Terrain->normalmap, "normalmap");
			shader->BindTexture(m_Terrain->splatmap, "splatmap");

			for (size_t i = 0; i < m_Terrain->materials.size(); i++)
			{
				if (m_Terrain->materials[i] == nullptr)
					continue;

				auto var = "materials[" + std::to_string(i) + "].";
				shader->BindTexture(m_Terrain->materials[i]->textureDiffuse, var + "diffusemap");
				shader->BindTexture(m_Terrain->materials[i]->textureNormal, var + "normalmap");
				shader->BindTexture(m_Terrain->materials[i]->textureHeight, var + "heightmap");

				shader->SetUniform1F(var + "heightScale", m_Terrain->materials[i]->heightScale);
				shader->SetUniform2Fv(var + "tiling", m_Terrain->materials[i]->tiling);
			}
		}
	}
	void TerrainMaterial::SetTerrain(Terrain* terrain)
	{
		m_Terrain = terrain;
	}
}