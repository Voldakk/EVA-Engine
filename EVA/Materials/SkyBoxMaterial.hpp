#pragma once

#include "StandardMaterial.hpp"
#include "../MaterialMap.hpp"

namespace EVA
{
	class Scene;

	class SkyBoxMaterial : public Material
	{
	public:

		std::shared_ptr<Texture> cubemap;

		glm::vec3 skyTint = glm::vec3(1.0f);
		glm::vec3 sunColor = glm::vec3(1.0f);
		float sunSize = 0.5f;

		SkyBoxMaterial() = default;

		void SetMaterialUniforms(Scene* scene) override;
	};
}