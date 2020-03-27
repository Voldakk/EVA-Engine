#include "TerrainMaterial.hpp"

#include "Terrain.hpp"

namespace EVA
{
	void TerrainMaterial::SetMaterialUniforms(Scene* scene) const
	{
		Material::SetMaterialUniforms(scene);

		if (m_Terrain != nullptr) 
		{
			
		}
	}

	void TerrainMaterial::SetTerrain(Terrain * terrain)
	{
		m_Terrain = terrain;
	}
}