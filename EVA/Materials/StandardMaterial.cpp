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
		glActiveTexture(GL_TEXTURE0);
		shader->SetUniform1I("material.texture_diffuse", 0);

		if (textureDiffuse != nullptr)
			glBindTexture(GL_TEXTURE_2D, textureDiffuse->id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultDiffuse->id);

		// Specular
		glActiveTexture(GL_TEXTURE1);
		shader->SetUniform1I("material.texture_specular", 1);

		if (textureSpecular != nullptr)
			glBindTexture(GL_TEXTURE_2D, textureSpecular->id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultSpecular->id);

		// Normal
		glActiveTexture(GL_TEXTURE2);
		shader->SetUniform1I("material.texture_normal", 2);

		if (textureNormal != nullptr)
			glBindTexture(GL_TEXTURE_2D, textureNormal->id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultNormal->id);

		// Emission
		glActiveTexture(GL_TEXTURE3);
		shader->SetUniform1I("material.texture_emission", 3);

		if (textureEmission != nullptr)
			glBindTexture(GL_TEXTURE_2D, textureEmission->id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultEmission->id);

		// Height
		glActiveTexture(GL_TEXTURE4);
		shader->SetUniform1I("material.texture_height", 4);

		if (textureHeight != nullptr)
			glBindTexture(GL_TEXTURE_2D, textureHeight->id);
		else
			glBindTexture(GL_TEXTURE_2D, textureDefaultSpecular->id);
	}

	void StandardMaterial::LoadAsset(const DataObject data)
	{
		Material::LoadAsset(data);

		// Shininess
		materialShininess = data.GetFloat("shininess", materialShininess);

		// Alpha cutoff
		alphaCutoff = data.GetFloat("alphaCutoff", alphaCutoff);

		// Tint
		tintDiffuse = data.GetVec4("tintDiffuse", tintDiffuse);

		// Diffuse
		const auto diffusePath = data.GetPath("textureDiffuse", "");
		if (!diffusePath.empty())
			SetTexture(Texture::Diffuse, diffusePath);

		// Specular
		const auto specularPath = data.GetPath("textureSpecular", "");
		if (!specularPath.empty())
			SetTexture(Texture::Specular, specularPath);

		// Normal
		const auto normalPath = data.GetPath("textureNormal", "");
		if (!normalPath.empty())
			SetTexture(Texture::Normal, normalPath);

		// Emission
		const auto emissionPath = data.GetPath("textureEmission", "");
		if (!emissionPath.empty())
			SetTexture(Texture::Emission, emissionPath);
	}

	void StandardMaterial::SaveAsset(DataObject & data) const
	{
		Material::SaveAsset(data);

		data.SetFloat("shininess", materialShininess);

		data.SetFloat("alphaCutoff", alphaCutoff);

		if (tintDiffuse != glm::vec4(1.0f))
			data.SetVec4("tintDiffuse", tintDiffuse);

		if (textureDiffuse != nullptr)
			data.SetPath("textureDiffuse", textureDiffuse->path);

		if (textureSpecular != nullptr)
			data.SetPath("textureSpecular", textureSpecular->path);

		if (textureNormal != nullptr)
			data.SetPath("textureNormal", textureNormal->path);

		if (textureEmission != nullptr)
			data.SetPath("textureEmission", textureEmission->path);
	}

	void StandardMaterial::DrawInspector()
	{
		Material::DrawInspector();

		if (InspectorFields::Float("Shininess", materialShininess))
			SaveToFile();

		if (InspectorFields::Float("Alpha cutoff", alphaCutoff))
			SaveToFile();

		std::string path;

		path = textureDiffuse == nullptr ? "" : FileSystem::ToString(textureDiffuse->path);
		if (InspectorFields::DragDropTargetString("Diffuse texture", path, "file"))
		{
			SetTexture(Texture::Diffuse, path);
			SaveToFile();
		}

		path = textureSpecular == nullptr ? "" : FileSystem::ToString(textureSpecular->path);
		if (InspectorFields::DragDropTargetString("Specular texture", path, "file"))
		{
			SetTexture(Texture::Specular, path);
			SaveToFile();
		}

		path = textureNormal == nullptr ? "" : FileSystem::ToString(textureNormal->path);
		if (InspectorFields::DragDropTargetString("Normal texture", path, "file"))
		{
			SetTexture(Texture::Normal, path);
			SaveToFile();
		}

		path = textureEmission == nullptr ? "" : FileSystem::ToString(textureEmission->path);
		if (InspectorFields::DragDropTargetString("Emission texture", path, "file"))
		{
			SetTexture(Texture::Emission, path);
			SaveToFile();
		}
	}
}
