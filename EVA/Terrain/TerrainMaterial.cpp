#include "TerrainMaterial.hpp"

#include "Terrain.hpp"

namespace EVA
{
	void TerrainMaterial::SetMaterialUniforms(Scene* scene) const
	{
		Material::SetMaterialUniforms(scene);

		if (m_Terrain != nullptr) 
		{
			shader->SetUniform1F("tessFactor", m_Terrain->tessFactor);
			shader->SetUniform1F("tessSlope", m_Terrain->tessSlope);
			shader->SetUniform1F("tessShift", m_Terrain->tessShift);
			shader->SetUniform1F("scaleY", m_Terrain->transform->scale.y);

			// Diffuse
			GLCall(glActiveTexture(GL_TEXTURE0));
			shader->SetUniform1I("material.texture_diffuse", 0);

			if (m_Terrain->heightmap != nullptr) {
				GLCall(glBindTexture(GL_TEXTURE_2D, m_Terrain->heightmap->id));
			}
			else {
				GLCall(glBindTexture(GL_TEXTURE_2D, 0));
			}
		}
	}

	void TerrainMaterial::SetTerrain(Terrain * terrain)
	{
		m_Terrain = terrain;
	}
}