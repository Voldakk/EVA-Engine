#include "ShadowMaterial.hpp"

#include "../Scene.hpp"
#include "../Application.hpp"

namespace EVA
{
	REGISTER_MATERIAL_CPP(SkyBoxMaterial, "EVA::SkyBoxMaterial")

	void SkyBoxMaterial::SetMaterialUniforms(Scene* scene) const
	{
		StandardMaterial::SetMaterialUniforms(scene);

		shader->SetUniformMatrix4Fv("view", Application::mainCamera->viewMatrix);
		shader->SetUniformMatrix4Fv("projection", Application::GetPerspectiveMatrix());

		shader->SetUniform4Fv("skyTint", skyTint);

		// Cubemap
		GLCall(glActiveTexture(GL_TEXTURE0));
		shader->SetUniform1I("material.texture_diffuse", 0);
	}

	void SkyBoxMaterial::SetTextures() const
	{
		if (textureDiffuse != nullptr)
			GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, textureDiffuse->id));
	}
}
