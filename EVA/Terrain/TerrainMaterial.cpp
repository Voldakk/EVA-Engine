#include "TerrainMaterial.hpp"

#include "Terrain.hpp"

namespace EVA
{
	TerrainMaterial::TerrainMaterial(Terrain* terrain) : m_Terrain(terrain)
	{

	}

	void TerrainMaterial::SetMaterialUniforms(Scene* scene) const
	{
		Material::SetMaterialUniforms(scene);

		shader->SetUniform1I("maxLod", m_Terrain->maxTessLod);
	}
}