#include "StandardMaterial.hpp"

#include "../Application.hpp"

namespace EVA
{
	bool StandardMaterial::init = true;
	std::shared_ptr<Texture> StandardMaterial::textureDefaultDiffuse;
	std::shared_ptr<Texture> StandardMaterial::textureDefaultSpecular;
	std::shared_ptr<Texture> StandardMaterial::textureDefaultNormal;
	std::shared_ptr<Texture> StandardMaterial::textureDefaultEmission;

	StandardMaterial::StandardMaterial()
	{
		if (init)
		{
			init = false;
			textureDefaultDiffuse = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_diffuse.png");
			textureDefaultSpecular = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_specular.png");
			textureDefaultNormal = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_normal.png");
			textureDefaultEmission = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_emission.png");
		}
	}

	void StandardMaterial::SetMaterialUniforms(Scene *scene) const
	{
		Material::SetMaterialUniforms(scene);

		// Material
		shader->SetUniform1F("material.shininess", materialShininess);
		shader->SetUniform4Fv("material.tint_diffuse", tintDiffuse);
		shader->SetUniform1F("material.alphaCutoff", alphaCutoff);

		// Textures
		SetTextures();
	}

	void StandardMaterial::SetTexture(const TextureType type, const FS::path& path)
	{
		const auto t = TextureManager::LoadTexture(path);
		if (t == nullptr)
		{
			switch (type)
			{
			case TextureType::Diffuse:
				textureDiffuse = nullptr;
				break;
			case TextureType::Specular:
				textureSpecular = nullptr;
				break;
			case TextureType::Normal:
				textureNormal = nullptr;
				break;
			case TextureType::Height:
				textureHeight = nullptr;
				break;
			case TextureType::Emission:
				textureEmission = nullptr;
				break;
			default:
				break;
			}
			return;
		}

		t->type = type;
		SetTexture(t);
	}

	void StandardMaterial::SetTexture(const std::shared_ptr<Texture>& texture)
	{
		switch (texture->type)
		{
		case TextureType::Diffuse:
			textureDiffuse = texture;
			break;
		case TextureType::Specular:
			textureSpecular = texture;
			break;
		case TextureType::Normal:
			textureNormal = texture;
			break;
		case TextureType::Height:
			textureHeight = texture;
			break;
		case TextureType::Emission:
			textureEmission = texture;
			break;
		default:
			break;
		}
	}

	void StandardMaterial::SetTextures() const
	{
		// Diffuse
		GLCall(glActiveTexture(GL_TEXTURE0));
		shader->SetUniform1I("material.texture_diffuse", 0);

		if (textureDiffuse != nullptr) {
			GLCall(glBindTexture(GL_TEXTURE_2D, textureDiffuse->id));
		}
		else {
			GLCall(glBindTexture(GL_TEXTURE_2D, textureDefaultDiffuse->id));
		}

		// Specular
		GLCall(glActiveTexture(GL_TEXTURE1));
		shader->SetUniform1I("material.texture_specular", 1);

		if (textureSpecular != nullptr) {
			GLCall(glBindTexture(GL_TEXTURE_2D, textureSpecular->id));
		}
		else {
			GLCall(glBindTexture(GL_TEXTURE_2D, textureDefaultSpecular->id));
		}

		// Normal
		GLCall(glActiveTexture(GL_TEXTURE2));
		shader->SetUniform1I("material.texture_normal", 2);

		if (textureNormal != nullptr) {
			GLCall(glBindTexture(GL_TEXTURE_2D, textureNormal->id));
		}
		else {
			GLCall(glBindTexture(GL_TEXTURE_2D, textureDefaultNormal->id));
		}

		// Emission
		GLCall(glActiveTexture(GL_TEXTURE3));
		shader->SetUniform1I("material.texture_emission", 3);

		if (textureEmission != nullptr) {
			GLCall(glBindTexture(GL_TEXTURE_2D, textureEmission->id));
		}
		else {
			GLCall(glBindTexture(GL_TEXTURE_2D, textureDefaultEmission->id));
		}

		// Height
		GLCall(glActiveTexture(GL_TEXTURE4));
		shader->SetUniform1I("material.texture_height", 4);

		if (textureHeight != nullptr) {
			GLCall(glBindTexture(GL_TEXTURE_2D, textureHeight->id));
		}
		else {
			GLCall(glBindTexture(GL_TEXTURE_2D, textureDefaultSpecular->id));
		}
	}

	//virtual void Serialize(DataObject& data) override;

	void StandardMaterial::Serialize(DataObject& data)
	{
		Material::Serialize(data);

		data.Serialize("tintDiffuse", tintDiffuse);
		data.Serialize("tiling", tiling);
		data.Serialize("heightScale", heightScale);
		data.Serialize("shininess", materialShininess);
		data.Serialize("alphaCutoff", alphaCutoff);

		auto path = textureDiffuse != nullptr ? textureDiffuse->path : "";

		if (data.Serialize("textureDiffuse", path) && !path.empty())
			SetTexture(TextureType::Diffuse, path);

		path = textureSpecular != nullptr ? textureSpecular->path : "";
		if (data.Serialize("textureSpecular", path) && !path.empty())
			SetTexture(TextureType::Specular, path);

		path = textureNormal != nullptr ? textureNormal->path : "";
		if (data.Serialize("textureNormal", path) && !path.empty())
			SetTexture(TextureType::Normal, path);

		path = textureEmission != nullptr ? textureEmission->path : "";
		if (data.Serialize("textureEmission", path) && !path.empty())
			SetTexture(TextureType::Emission, path);

		path = textureHeight != nullptr ? textureHeight->path : "";
		if (data.Serialize("textureHeight", path) && !path.empty())
			SetTexture(TextureType::Height, path);
	}
}
