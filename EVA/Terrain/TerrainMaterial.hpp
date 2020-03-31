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

		void SetObjectUniforms(Transform* transform) override;
		void SetTerrain(Terrain* terrain);
	};
}
