#pragma once

#include "../Material.hpp"
#include "../MaterialMap.hpp"

namespace EVA
{
	class Water;

	class WaterMaterial : public Material
	{
		Water* m_Water;

	public:

		void SetMaterialUniforms(Scene* scene) override;
		void SetWater(Water* water);
	};
}