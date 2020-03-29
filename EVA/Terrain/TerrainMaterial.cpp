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

			shader->BindTexture(m_Terrain->heightmap, "heightmap", 0);
			shader->BindTexture(m_Terrain->normalmap, "normalmap", 1);
		}
	}

	void TerrainMaterial::SetTerrain(Terrain * terrain)
	{
		m_Terrain = terrain;
	}
}