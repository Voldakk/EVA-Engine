#pragma once

#include "StandardMaterial.hpp"
#include "../MaterialMap.hpp"

namespace EVA
{
	class Scene;

	class SkyBoxMaterial : public StandardMaterial
	{
		REGISTER_MATERIAL(SkyBoxMaterial, "EVA::SkyBoxMaterial")

	public:

		glm::vec4 skyTint = glm::vec4(1.0f);

		SkyBoxMaterial() = default;

		void SetMaterialUniforms(Scene* scene) override;
		void SetTextures() override;
	};
}