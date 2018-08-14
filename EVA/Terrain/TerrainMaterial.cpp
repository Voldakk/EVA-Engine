#include "TerrainMaterial.hpp"

#include "Terrain.hpp"

namespace EVA
{
	REGISTER_MATERIAL_CPP(TerrainMaterial, "EVA::TerrainMaterial")

	void TerrainMaterial::SetMaterialUniforms(Scene* scene) const
	{
		Material::SetMaterialUniforms(scene);

		if(m_Terrain != nullptr)
			shader->SetUniform1I("maxLod", m_Terrain->maxTessLod);
	}

	void TerrainMaterial::SetTerrain(Terrain * terrain)
	{
		m_Terrain = terrain;
	}
}