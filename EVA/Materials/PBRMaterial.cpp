#include "PBRMaterial.hpp"

namespace EVA
{
	PBRMaterial::PBRMaterial()
	{
		if (init)
		{
			init = false;
			defaultAlbedoMap = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_albedo.png");
			defaultMetallicMap = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_metallic.png");
			defaultRoughnessMap = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_roughness.png");
			defaultAOMap = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_ao.png");
			defaultNormalMap = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_normal.png");
			defaultHeightMap = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_height.png");
		}
	}

	void PBRMaterial::SetMaterialUniforms(Scene* scene)
	{
		Material::SetMaterialUniforms(scene);

		shader->SetUniform4Fv("tint", tint);
		shader->SetUniform2Fv("tiling", tiling);
		shader->SetUniform1F("heightScale", heightScale);

		shader->BindTexture(GetAlbedoMap(), "albedoMap");
		shader->BindTexture(GetMetallicMap(), "metallicMap");
		shader->BindTexture(GetRoughnessMap(), "roughnessMap");
		shader->BindTexture(GetAOMap(), "aoMap");
		shader->BindTexture(GetNormalMap(), "normalMap");
		shader->BindTexture(GetHeightMap(), "heightMap");
	}

	void PBRMaterial::Serialize(DataObject& data)
	{
		Material::Serialize(data);

		data.Serialize("tiling", tiling);
		data.Serialize("tint", tint);
		data.Serialize("heightScale", heightScale);

		auto path = albedoMap != nullptr ? albedoMap->path : "";
		if (data.Serialize("albedoMap", path) && !path.empty())
			albedoMap = TextureManager::LoadTexture(path);

		path = metallicMap != nullptr ? metallicMap->path : "";
		if (data.Serialize("metallicMap", path) && !path.empty())
			metallicMap = TextureManager::LoadTexture(path);

		path = roughnessMap != nullptr ? roughnessMap->path : "";
		if (data.Serialize("roughnessMap", path) && !path.empty())
			roughnessMap = TextureManager::LoadTexture(path);

		path = aoMap != nullptr ? aoMap->path : "";
		if (data.Serialize("aoMap", path) && !path.empty())
			aoMap = TextureManager::LoadTexture(path);

		path = normalMap != nullptr ? normalMap->path : "";
		if (data.Serialize("normalMap", path) && !path.empty())
			normalMap = TextureManager::LoadTexture(path);

		path = heightMap != nullptr ? heightMap->path : "";
		if (data.Serialize("heightMap", path) && !path.empty())
			heightMap = TextureManager::LoadTexture(path);
	}

	std::shared_ptr<Texture> PBRMaterial::GetAlbedoMap()
	{
		return albedoMap != nullptr ? albedoMap : defaultAlbedoMap;
	}
	std::shared_ptr<Texture> PBRMaterial::GetMetallicMap()
	{
		return metallicMap != nullptr ? metallicMap : defaultMetallicMap;
	}
	std::shared_ptr<Texture> PBRMaterial::GetRoughnessMap()
	{
		return roughnessMap != nullptr ? roughnessMap : defaultRoughnessMap;
	}
	std::shared_ptr<Texture> PBRMaterial::GetAOMap()
	{
		return aoMap != nullptr ? aoMap : defaultAOMap;
	}
	std::shared_ptr<Texture> PBRMaterial::GetNormalMap()
	{
		return normalMap != nullptr ? normalMap : defaultNormalMap;
	}
	std::shared_ptr<Texture> PBRMaterial::GetHeightMap()
	{
		return heightMap != nullptr ? heightMap : defaultHeightMap;
	}
}