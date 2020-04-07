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
		inline static std::shared_ptr<Texture> defaultHeightMap;

		std::shared_ptr<Texture> albedoMap;
		std::shared_ptr<Texture> metallicMap;
		std::shared_ptr<Texture> roughnessMap;
		std::shared_ptr<Texture> aoMap;
		std::shared_ptr<Texture> normalMap;
		std::shared_ptr<Texture> heightMap;

		glm::vec2 tiling = glm::vec2(1.0f);
		glm::vec4 tint = glm::vec4(1.0f);
		float heightScale = 1.0f;

		PBRMaterial();

		void SetMaterialUniforms(Scene* scene) override;

		virtual void Serialize(DataObject& data) override;

		std::shared_ptr<Texture> GetAlbedoMap();
		std::shared_ptr<Texture> GetMetallicMap();
		std::shared_ptr<Texture> GetRoughnessMap();
		std::shared_ptr<Texture> GetAOMap();
		std::shared_ptr<Texture> GetNormalMap();
		std::shared_ptr<Texture> GetHeightMap();
	};
}