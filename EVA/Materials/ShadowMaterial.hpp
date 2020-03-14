#pragma once

#include "../Material.hpp"
#include "../MaterialMap.hpp"

namespace EVA
{
	class Scene;

	class ShadowMaterial : public Material
	{
		REGISTER_MATERIAL(ShadowMaterial, "EVA::ShadowMaterial")

	public:
		void SetMaterialUniforms(Scene* scene) const override;
	};
}