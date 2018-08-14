#pragma once

#include "../Material.hpp"
#include "../MaterialMap.hpp"

namespace EVA
{

	class Terrain;

	class TerrainMaterial : public Material
	{
		REGISTER_MATERIAL_HPP(TerrainMaterial)
		Terrain* m_Terrain;
		
	public:

		void SetMaterialUniforms(Scene* scene) const override;

		void SetTerrain(Terrain* terrain);
	};
}
