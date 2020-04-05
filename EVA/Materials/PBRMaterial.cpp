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
		}
	}

	void PBRMaterial::SetMaterialUniforms(Scene* scene)
	{
		Material::SetMaterialUniforms(scene);

		shader->SetUniform4Fv("tint", tint);
		shader->SetUniform2Fv("tiling", tiling);

		shader->BindTexture(albedoMap != nullptr ? albedoMap : defaultAlbedoMap, "albedoMap");
		shader->BindTexture(metallicMap != nullptr ? metallicMap : defaultMetallicMap, "metallicMap");
		shader->BindTexture(roughnessMap != nullptr ? roughnessMap : defaultRoughnessMap, "roughnessMap");
		shader->BindTexture(aoMap != nullptr ? aoMap : defaultAOMap, "aoMap");
		shader->BindTexture(normalMap != nullptr ? normalMap : defaultNormalMap, "normalMap");
	}

	void PBRMaterial::Serialize(DataObject& data)
	{
		Material::Serialize(data);

		data.Serialize("tintDiffuse", tint);
		data.Serialize("tiling", tiling);

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
	}
}