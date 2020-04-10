#include "ShadowMaterial.hpp"

#include "../Scene.hpp"
#include "../Application.hpp"

namespace EVA
{
	void SkyBoxMaterial::SetMaterialUniforms(Scene* scene)
	{
		Material::SetMaterialUniforms(scene);

		shader->SetUniformMatrix4Fv("view", Application::mainCamera->viewMatrix);
		shader->SetUniformMatrix4Fv("projection", Application::GetPerspectiveMatrix());

		shader->SetUniform3Fv("skyTint", skyTint);
		shader->SetUniform3Fv("sunColor", sunColor);
		shader->SetUniform1F("sunSize", sunSize);

		shader->BindTexture(cubemap, "skytexture");
	}
}
