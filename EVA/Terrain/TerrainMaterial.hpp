#pragma once

#include "../Material.hpp"
#include "../MaterialMap.hpp"

namespace EVA
{
	class Terrain;

	class TerrainMaterial : public Material
	{
		REGISTER_MATERIAL(TerrainMaterial, "EVA::TerrainMaterial")

		Terrain* m_Terrain;
		
	public:

		void SetMaterialUniforms(Scene* scene) override;
		void SetTerrain(Terrain* terrain);
	};
}
