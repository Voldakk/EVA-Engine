#include "StandardMaterial.hpp"

#include "../Application.hpp"

namespace EVA
{
	REGISTER_MATERIAL_CPP(StandardMaterial, "EVA::StandardMaterial")

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

	void StandardMaterial::SetMaterialUniforms(Scene* scene) const
	{
		Material::SetMaterialUniforms(scene);

		// Material
		shader->SetUniform1F("material.shininess", materialShininess);
		shader->SetUniform4Fv("material.tint_diffuse", tintDiffuse);
		shader->SetUniform1F("material.alphaCutoff", alphaCutoff);

		// Textures
		SetTextures();
	}

	void StandardMaterial::SetTexture(const Texture::Type type, const FS::path& path)
	{
		const auto t = TextureManager::LoadTexture(path);
		if (t == nullptr)
		{
			switch (type)
			{
			case Texture::Diffuse:
				textureDiffuse = nullptr;
				break;
			case Texture::Specular:
				textureSpecular = nullptr;
				break;
			case Texture::Normal:
				textureNormal = nullptr;
				break;
			case Texture::Height:
				textureHeight = nullptr;
				break;
			case Texture::Emission:
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
		case Texture::Diffuse:
			textureDiffuse = texture;
			break;
		case Texture::Specular:
			textureSpecular = texture;
			break;
		case Texture::Normal:
			textureNormal = texture;
			break;
		case Texture::Height:
			textureHeight = texture;
			break;
		case Texture::Emission:
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

		if (textureDiffuse != nullptr)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, textureDiffuse->id));
		}
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, textureDefaultDiffuse->id));
		}

		// Specular
		GLCall(glActiveTexture(GL_TEXTURE1));
		shader->SetUniform1I("material.texture_specular", 1);

		if (textureSpecular != nullptr)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, textureSpecular->id));
		}
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, textureDefaultSpecular->id));
		}

		// Normal
		GLCall(glActiveTexture(GL_TEXTURE2));
		shader->SetUniform1I("material.texture_normal", 2);

		if (textureNormal != nullptr)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, textureNormal->id));
		}
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, textureDefaultNormal->id));
		}

		// Emission
		GLCall(glActiveTexture(GL_TEXTURE3));
		shader->SetUniform1I("material.texture_emission", 3);

		if (textureEmission != nullptr)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, textureEmission->id));
		}
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, textureDefaultEmission->id));
		}

		// Height
		GLCall(glActiveTexture(GL_TEXTURE4));
		shader->SetUniform1I("material.texture_height", 4);

		if (textureHeight != nullptr)
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, textureHeight->id));
		}
		else
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, textureDefaultSpecular->id));
		}
	}

	//virtual void Serialize(DataObject& data) override;

	void StandardMaterial::Serialize(DataObject& data)
	{
		Material::Serialize(data);

		data.Serialize("shininess", materialShininess);
		data.Serialize("alphaCutoff", alphaCutoff);
		data.Serialize("tintDiffuse", tintDiffuse);

		auto path = textureDiffuse != nullptr ? textureDiffuse->path : "";

		if (data.Serialize("textureDiffuse", path) && !path.empty())
			SetTexture(Texture::Diffuse, path);

		path = textureSpecular != nullptr ? textureSpecular->path : "";
		if (data.Serialize("textureSpecular", path) && !path.empty())
			SetTexture(Texture::Specular, path);

		path = textureNormal != nullptr ? textureNormal->path : "";
		if (data.Serialize("textureNormal", path) && !path.empty())
			SetTexture(Texture::Normal, path);

		path = textureEmission != nullptr ? textureEmission->path : "";
		if (data.Serialize("textureEmission", path) && !path.empty())
			SetTexture(Texture::Emission, path);
	}
}
