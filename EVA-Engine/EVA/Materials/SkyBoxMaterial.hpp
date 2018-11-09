#pragma once

#include "StandardMaterial.hpp"
#include "../MaterialMap.hpp"

namespace EVA
{
	class Scene;

	class EVA_API SkyBoxMaterial : public StandardMaterial
	{
	REGISTER_MATERIAL_HPP(SkyBoxMaterial)

	public:

		glm::vec4 skyTint = glm::vec4(1.0f);

		SkyBoxMaterial() = default;

		void SetMaterialUniforms(Scene* scene) const override;
		void SetTextures() const override;
	};
}