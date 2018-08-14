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
		glActiveTexture(GL_TEXTURE0);
		shader->SetUniform1I("material.texture_diffuse", 0);

		if (textureDiffuse != nullptr)
			glBindTexture(GL_TEXTURE_CUBE_MAP, textureDiffuse->id);
	}
}
