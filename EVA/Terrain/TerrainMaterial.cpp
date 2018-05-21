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

		shader->SetUniform1I("tessLevelOuter", m_Terrain->tessLevelOuter);
		shader->SetUniform1I("tessLevelInner0", m_Terrain->tessLevelInner0);
		shader->SetUniform1I("tessLevelInner1", m_Terrain->tessLevelInner1);
	}
}