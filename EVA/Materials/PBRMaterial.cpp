#include "PBRMaterial.hpp"

#include "../Scene.hpp"

namespace EVA
{
	PBRMaterial::PBRMaterial()
	{
		if (init)
		{
			init = false;
			defaultAlbedoMap = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_albedo.png");
			defaultMetRouAoMap = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_met_rou_ao.png");
			defaultNormalHeightMap = TextureManager::LoadTexture(DEFAULT_TEXTURES_PATH / "default_normal_height.tga");
		}
	}

	void PBRMaterial::SetMaterialUniforms(Scene* scene)
	{
		Material::SetMaterialUniforms(scene);

		shader->SetUniform4Fv("tint", tint);
		shader->SetUniform2Fv("tiling", tiling);
		shader->SetUniform1F("heightScale", heightScale);

		shader->BindTexture(GetAlbedoMap(), "albedoMap");
		shader->BindTexture(GetMetRouAoMap(), "metRouAoMap");
		shader->BindTexture(GetNormalHeightMap(), "normalHeightMap");

		if (scene != nullptr && scene->skybox != nullptr)
		{
			shader->BindTexture(scene->skybox->irradianceMap, "irradianceMap");
			shader->BindTexture(scene->skybox->preFilterMap, "prefilterMap");
			shader->BindTexture(scene->skybox->preComputedBRDF, "brdfLUT");
		}
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

		path = metRouAoMap != nullptr ? metRouAoMap->path : "";
		if (data.Serialize("metRouAoMap", path) && !path.empty())
			metRouAoMap = TextureManager::LoadTexture(path);

		path = normalHeightMap != nullptr ? normalHeightMap->path : "";
		if (data.Serialize("normalHeightMap", path) && !path.empty())
			normalHeightMap = TextureManager::LoadTexture(path);
	}

	std::shared_ptr<Texture> PBRMaterial::GetAlbedoMap()
	{
		return albedoMap != nullptr ? albedoMap : defaultAlbedoMap;
	}
	std::shared_ptr<Texture> PBRMaterial::GetMetRouAoMap()
	{
		return metRouAoMap != nullptr ? metRouAoMap : defaultMetRouAoMap;
	}
	std::shared_ptr<Texture> PBRMaterial::GetNormalHeightMap()
	{
		return normalHeightMap != nullptr ? normalHeightMap : defaultNormalHeightMap;
	}
}