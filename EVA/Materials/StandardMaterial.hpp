#pragma once

#include "../Material.hpp"
#include "../MaterialMap.hpp"

namespace EVA
{
	class Scene;

	class StandardMaterial : public Material
	{
		REGISTER_MATERIAL_HPP(StandardMaterial)

		inline static const FS::path DEFAULT_TEXTURES_PATH = "./assets/standard assets/textures/";

	public:

		static bool init;
		static std::shared_ptr<Texture> textureDefaultDiffuse;
		static std::shared_ptr<Texture> textureDefaultSpecular;
		static std::shared_ptr<Texture> textureDefaultNormal;
		static std::shared_ptr<Texture> textureDefaultEmission;

		std::shared_ptr<Texture> textureDiffuse;
		std::shared_ptr<Texture> textureSpecular;
		std::shared_ptr<Texture> textureNormal;
		std::shared_ptr<Texture> textureEmission;
		std::shared_ptr<Texture> textureHeight;

		glm::vec4 tintDiffuse = glm::vec4(1.0f);

		float materialShininess = 32.0f;
		float alphaCutoff = 0.0f;

		StandardMaterial();

		void SetMaterialUniforms(Scene* scene) const override;
		
		void SetTexture(Texture::Type type, const FS::path& path);

		void SetTexture(const std::shared_ptr<Texture>& texture);

		void SetTextures() const;

		/**
		* \brief Loads Material values from the given DataObject
		* \param data The DataObject
		*/
		void Load(const DataObject data) override;

		/**
		* \brief Saves Material values to the given DataObject
		* \param data The DataObject
		*/
		void Save(DataObject& data) const override;

		void Inspector() override;
	};
}
