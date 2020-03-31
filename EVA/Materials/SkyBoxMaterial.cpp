#include "ShadowMaterial.hpp"

#include "../Scene.hpp"
#include "../Application.hpp"

namespace EVA
{
	void SkyBoxMaterial::SetMaterialUniforms(Scene* scene)
	{
		StandardMaterial::SetMaterialUniforms(scene);

		shader->SetUniformMatrix4Fv("view", Application::mainCamera->viewMatrix);
		shader->SetUniformMatrix4Fv("projection", Application::GetPerspectiveMatrix());

		shader->SetUniform4Fv("skyTint", skyTint);
	}

	void SkyBoxMaterial::SetTextures()
	{
		shader->BindTexture(textureDiffuse, "material.texture_diffuse", TextureTarget::TextureCubeMap);
	}
}
