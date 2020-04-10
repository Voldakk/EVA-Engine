#include "BlinnPhongMaterial.hpp"

#include "../Application.hpp"

namespace EVA
{
	bool BlinnPhongMaterial::init = true;
	std::shared_ptr<Texture> BlinnPhongMaterial::defaultDiffuseMap;
	std::shared_ptr<Texture> BlinnPhongMaterial::defaultSpecularMap;
	std::shared_ptr<Texture> BlinnPhongMaterial::defaultNormalMap;
	std::shared_ptr<Texture> BlinnPhongMaterial::defaultEmissionMap;
	std::shared_ptr<Texture> BlinnPhongMaterial::defaultHeightMap;

	BlinnPhongMaterial::BlinnPhongMaterial()
	{
		if (init)
		{
			init = false;
			defaultDiffuseMap = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_diffuse.png");
			defaultSpecularMap = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_specular.png");
			defaultNormalMap = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_normal.png");
			defaultEmissionMap = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_emission.png");
			defaultHeightMap = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_height.png");
		}
	}

	void BlinnPhongMaterial::SetMaterialUniforms(Scene *scene)
	{
		Material::SetMaterialUniforms(scene);

		// Material
		shader->SetUniform2Fv("tiling", tiling);
		shader->SetUniform4Fv("tintDiffuse", tintDiffuse);
		shader->SetUniform1F("shininess", materialShininess);
		shader->SetUniform1F("heightScale", heightScale);
		shader->SetUniform1F("alphaCutoff", alphaCutoff);

		// Textures
		shader->BindTexture(GetDiffuseMap(), "diffuseMap");
		shader->BindTexture(GetSpecularMap(), "specularMap");
		shader->BindTexture(GetNormalMap(), "normalMap");
		shader->BindTexture(GetEmissionMap(), "emissionMap");
		shader->BindTexture(GetHeightMap(), "heightMap");
	}

	void BlinnPhongMaterial::Serialize(DataObject& data)
	{
		Material::Serialize(data);

		data.Serialize("tintDiffuse", tintDiffuse);
		data.Serialize("tiling", tiling);
		data.Serialize("shininess", materialShininess);
		data.Serialize("heightScale", heightScale);
		data.Serialize("alphaCutoff", alphaCutoff);

		auto path = diffuseMap != nullptr ? diffuseMap->path : "";
		if (data.Serialize("diffuseMap", path) && !path.empty())
			diffuseMap = TextureManager::LoadTexture(path);

		path = specularMap != nullptr ? specularMap->path : "";
		if (data.Serialize("specularMap", path) && !path.empty())
			specularMap = TextureManager::LoadTexture(path);

		path = normalMap != nullptr ? normalMap->path : "";
		if (data.Serialize("normalMap", path) && !path.empty())
			normalMap = TextureManager::LoadTexture(path);

		path = emissionMap != nullptr ? emissionMap->path : "";
		if (data.Serialize("emissionMap", path) && !path.empty())
			emissionMap = TextureManager::LoadTexture(path);

		path = heightMap != nullptr ? heightMap->path : "";
		if (data.Serialize("heightMap", path) && !path.empty())
			heightMap = TextureManager::LoadTexture(path);
	}

	std::shared_ptr<Texture> BlinnPhongMaterial::GetDiffuseMap()
	{
		return diffuseMap != nullptr ? diffuseMap : defaultDiffuseMap;
	}
	std::shared_ptr<Texture> BlinnPhongMaterial::GetSpecularMap()
	{
		return specularMap != nullptr ? specularMap : defaultSpecularMap;
	}
	std::shared_ptr<Texture> BlinnPhongMaterial::GetNormalMap()
	{
		return normalMap != nullptr ? normalMap : defaultNormalMap;
	}
	std::shared_ptr<Texture> BlinnPhongMaterial::GetEmissionMap()
	{
		return emissionMap != nullptr ? emissionMap : defaultEmissionMap;
	}
	std::shared_ptr<Texture> BlinnPhongMaterial::GetHeightMap()
	{
		return heightMap != nullptr ? heightMap : defaultHeightMap;
	}
}
