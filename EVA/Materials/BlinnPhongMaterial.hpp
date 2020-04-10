#pragma once

#include "../Material.hpp"
#include "../MaterialMap.hpp"

namespace EVA
{
	class Scene;

	class BlinnPhongMaterial : public Material
	{
		REGISTER_MATERIAL(BlinnPhongMaterial, "EVA::BlinnPhongMaterial")

		inline static const FS::path DEFAULT_TEXTURES_PATH = "./assets/standard assets/textures/";

	public:

		static bool init;
		static std::shared_ptr<Texture> defaultDiffuseMap;
		static std::shared_ptr<Texture> defaultSpecularMap;
		static std::shared_ptr<Texture> defaultNormalMap;
		static std::shared_ptr<Texture> defaultEmissionMap;
		static std::shared_ptr<Texture> defaultHeightMap;

		std::shared_ptr<Texture> diffuseMap;
		std::shared_ptr<Texture> specularMap;
		std::shared_ptr<Texture> normalMap;
		std::shared_ptr<Texture> emissionMap;
		std::shared_ptr<Texture> heightMap;

		glm::vec4 tintDiffuse = glm::vec4(1.0f);

		glm::vec2 tiling = glm::vec2(1.0f);
		float heightScale = 1.0f;
		float materialShininess = 32.0f;
		float alphaCutoff = 0.0f;

		BlinnPhongMaterial();

		void SetMaterialUniforms(Scene* scene) override;

		virtual void Serialize(DataObject& data) override;

		std::shared_ptr<Texture> GetDiffuseMap();
		std::shared_ptr<Texture> GetSpecularMap();
		std::shared_ptr<Texture> GetNormalMap();
		std::shared_ptr<Texture> GetEmissionMap();
		std::shared_ptr<Texture> GetHeightMap();
	};
}
