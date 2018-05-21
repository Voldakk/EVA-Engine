#pragma once

#include "../Material.hpp"

namespace EVA
{

	class Terrain;

	class TerrainMaterial : public Material
	{
		Terrain* m_Terrain;
		
	public:

		explicit TerrainMaterial(Terrain* terrain);

		void SetMaterialUniforms(Scene* scene) const override;
	};
}
