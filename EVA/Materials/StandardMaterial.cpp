#include "StandardMaterial.hpp"

#include "../Application.hpp"

namespace EVA
{
	bool StandardMaterial::init = true;
	std::shared_ptr<Texture> StandardMaterial::textureDefaultDiffuse;
	std::shared_ptr<Texture> StandardMaterial::textureDefaultSpecular;
	std::shared_ptr<Texture> StandardMaterial::textureDefaultNormal;
	std::shared_ptr<Texture> StandardMaterial::textureDefaultEmission;
	std::shared_ptr<Texture> StandardMaterial::textureDefaultHeight;

	StandardMaterial::StandardMaterial()
	{
		if (init)
		{
			init = false;
			textureDefaultDiffuse = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_diffuse.png");
			textureDefaultSpecular = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_specular.png");
			textureDefaultNormal = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_normal.png");
			textureDefaultEmission = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_emission.png");
			textureDefaultHeight = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_height.png");
		}
	}

	void StandardMaterial::SetMaterialUniforms(Scene *scene)
	{
		Material::SetMaterialUniforms(scene);

		// Material
		shader->SetUniform2Fv("material.tiling", tiling);
		shader->SetUniform1F("material.heightScale", heightScale);
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

	void StandardMaterial::SetTextures()
	{
		// Diffuse
		if (textureDiffuse != nullptr)
			shader->BindTexture(textureDiffuse, "material.texture_diffuse");
		else
			shader->BindTexture(textureDefaultDiffuse, "material.texture_diffuse");

		// Specular
		if (textureSpecular != nullptr)
			shader->BindTexture(textureSpecular, "material.texture_specular");
		else
			shader->BindTexture(textureDefaultSpecular, "material.texture_specular");

		// Normal
		if (textureNormal != nullptr)
			shader->BindTexture(textureNormal, "material.texture_normal");
		else
			shader->BindTexture(textureDefaultNormal, "material.texture_normal");

		// Emission
		if (textureEmission != nullptr)
			shader->BindTexture(textureEmission, "material.texture_emission");
		else
			shader->BindTexture(textureDefaultEmission, "material.texture_emission");

		// Height
		if (textureHeight != nullptr)
			shader->BindTexture(textureHeight, "material.texture_height");
		else
			shader->BindTexture(textureDefaultHeight, "material.texture_height");
	}

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
