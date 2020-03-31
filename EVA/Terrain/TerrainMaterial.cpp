#include "TerrainMaterial.hpp"

#include "Terrain.hpp"

namespace EVA
{
	void TerrainMaterial::SetMaterialUniforms(Scene* scene) const
	{
		Material::SetMaterialUniforms(scene);

		if (m_Terrain != nullptr) 
		{
			int unit = 0;

			shader->SetUniform1F("tessFactor", m_Terrain->tessFactor);
			shader->SetUniform1F("tessSlope", m_Terrain->tessSlope);
			shader->SetUniform1F("tessShift", m_Terrain->tessShift);
			shader->SetUniform1F("scaleY", m_Terrain->transform->scale.y);

			shader->BindTexture(m_Terrain->heightmap, "heightmap", unit++);
			shader->BindTexture(m_Terrain->normalmap, "normalmap", unit++);

			for (size_t i = 0; i < m_Terrain->materials.size(); i++)
			{
				if (m_Terrain->materials[i] == nullptr)
					continue;

				auto var = "materials[" + std::to_string(i) + "].";
				shader->BindTexture(m_Terrain->materials[i]->textureDiffuse, var + "diffusemap", unit++);
				shader->BindTexture(m_Terrain->materials[i]->textureNormal, var + "normalmap", unit++);
				shader->BindTexture(m_Terrain->materials[i]->textureHeight, var + "heightmap", unit++);

				shader->SetUniform1F(var + "heightScale", m_Terrain->materials[i]->heightScale);
				shader->SetUniform2Fv(var + "tiling", m_Terrain->materials[i]->tiling);
			}
		}
	}

	void TerrainMaterial::SetTerrain(Terrain * terrain)
	{
		m_Terrain = terrain;
	}
}