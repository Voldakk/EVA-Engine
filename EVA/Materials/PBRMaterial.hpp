#pragma once

#include "../Material.hpp"
#include "../MaterialMap.hpp"

namespace EVA
{
	class Scene;

	class PBRMaterial : public Material
	{
		REGISTER_MATERIAL(PBRMaterial, "EVA::PBRMaterial")

			inline static const FS::path DEFAULT_TEXTURES_PATH = "./assets/standard assets/textures/";

	public:

		inline static bool init = true;
		inline static std::shared_ptr<Texture> defaultAlbedoMap;
		inline static std::shared_ptr<Texture> defaultMetallicMap;
		inline static std::shared_ptr<Texture> defaultRoughnessMap;
		inline static std::shared_ptr<Texture> defaultAOMap;
		inline static std::shared_ptr<Texture> defaultNormalMap;

		std::shared_ptr<Texture> albedoMap;
		std::shared_ptr<Texture> metallicMap;
		std::shared_ptr<Texture> roughnessMap;
		std::shared_ptr<Texture> aoMap;
		std::shared_ptr<Texture> normalMap;

		glm::vec4 tint = glm::vec4(1.0f);
		glm::vec2 tiling = glm::vec2(1.0f);

		PBRMaterial();

		void SetMaterialUniforms(Scene* scene) override;

		virtual void Serialize(DataObject& data) override;
	};
}