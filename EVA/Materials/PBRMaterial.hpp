#pragma once

#include "../Material.hpp"
#include "../MaterialMap.hpp"

namespace EVA
{
	class Scene;

	class PBRMaterial : public Material
	{
		REGISTER_MATERIAL(PBRMaterial, "EVA::PBRMaterial")

		inline static const FS::path DEFAULT_TEXTURES_PATH = "./assets/standard_assets/textures/";

	public:

		inline static bool init = true;
		inline static std::shared_ptr<Texture> defaultAlbedoMap;
		inline static std::shared_ptr<Texture> defaultMetRouAoMap;
		inline static std::shared_ptr<Texture> defaultNormalHeightMap;

		std::shared_ptr<Texture> albedoMap;
		std::shared_ptr<Texture> metRouAoMap;
		std::shared_ptr<Texture> normalHeightMap;

		glm::vec2 tiling = glm::vec2(1.0f);
		glm::vec4 tint = glm::vec4(1.0f);
		float heightScale = 1.0f;

		PBRMaterial();

		void SetMaterialUniforms(Scene* scene) override;

		virtual void Serialize(DataObject& data) override;

		std::shared_ptr<Texture> GetAlbedoMap();
		std::shared_ptr<Texture> GetMetRouAoMap();
		std::shared_ptr<Texture> GetNormalHeightMap();
	};
}