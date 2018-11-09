#pragma once

#include "../Material.hpp"
#include "../MaterialMap.hpp"

namespace EVA
{
	class Scene;

	class EVA_API ShadowMaterial : public Material
	{
	REGISTER_MATERIAL_HPP(ShadowMaterial)

	public:
		void SetMaterialUniforms(Scene* scene) const override;
	};
}
