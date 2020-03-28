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
		}
	}

	void TerrainMaterial::SetTerrain(Terrain * terrain)
	{
		m_Terrain = terrain;
	}
}