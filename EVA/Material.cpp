#include "Material.hpp"

#include "Scene.hpp"
#include "Application.hpp"

namespace EVA
{

	Material *Material::m_ActiveMaterial;

	unsigned int Material::textureBlack;

	Material::Material() = default;

	void Material::SetTexture(const TextureType type, const char *path)
	{
		Texture t = {0, type, path};
		t.id = TextureManager::GetTexture(t.path);
		SetTexture(t);
	}

	void Material::SetTexture(const TextureType type, const unsigned int id)
	{
		const Texture t = {id, type, ""};
		SetTexture(t);
	}

	void Material::SetTexture(const Texture texture)
	{
		switch (texture.type)
		{
		case Diffuse: 
			textureDiffuse = texture;
			break;
		case Specular: 
			textureSpecular = texture;
			break;
		case Normal: 
			textureNormal = texture;
			break;
		case Height: 
			textureHeight = texture;
			break;
		case Emission: 
			textureEmission = texture;
			break;
		default: 
			break;
		}
	}

	void Material::Activate(Scene *scene, Transform *transform)
	{
		if (m_ActiveMaterial != this)
		{
			m_ActiveMaterial = this;

			shader->Bind();

			SetMaterialUniforms(scene);
		}

		if (transform != nullptr)
			SetObjectUniforms(transform);
	}

	void Material::SetMaterialUniforms(Scene *scene)
	{
		// Material
		shader->SetUniform1f("material.shininess", materialShininess);

		// Textures
		SetTextures();

		// Camera
		shader->SetUniform3fv("cameraPosition", Application::m_MainCamera->GetGameObject()->GetTransform()->position);

		// View and projection
		shader->SetUniformMatrix4fv("view", Application::m_MainCamera->GetViewMatrix());
		shader->SetUniformMatrix4fv("projection", Application::GetPerspectiveMatrix());

		if (scene == nullptr)
			return;

		// Lights

		auto lights = scene->GetLights();
		shader->SetUniform1i("numLights", lights.size());

		for (unsigned int i = 0; i < lights.size(); ++i)
		{
			const auto lightNum = "allLights[" + std::to_string(i) + "].";

            shader->SetUniform3fv(lightNum + "color", lights[i]->Color);
            shader->SetUniform1f(lightNum + "ambientCoefficient", lights[i]->AmbientCoefficient);

            switch (lights[i]->Type)
            {

                case LightType::Directional:


                    shader->SetUniform4fv(lightNum + "position", lights[i]->GetDirection());
                    break;

                case LightType::Point:
                    shader->SetUniform4fv(lightNum + "position", glm::vec4(lights[i]->Position, 1.0f));
                    shader->SetUniform1f(lightNum + "attenuation", lights[i]->Attenuation);
                    break;
            }
		}
	}

	void Material::SetObjectUniforms(Transform *transform) const
	{
		// Position
		shader->SetUniformMatrix4fv("model", transform->GetModelMatrix());
	}

	void Material::SetTextures() const
	{
		// Diffuse
		glActiveTexture(GL_TEXTURE0);
		shader->SetUniform1i("material.texture_diffuse", 0);

		if (textureDiffuse.id != 0)
			glBindTexture(GL_TEXTURE_2D, textureDiffuse.id);
		else
			glBindTexture(GL_TEXTURE_2D, textureBlack);

		// Diffuse
		glActiveTexture(GL_TEXTURE1);
		shader->SetUniform1i("material.texture_specular", 1);

		if (textureSpecular.id != 0)
			glBindTexture(GL_TEXTURE_2D, textureSpecular.id);
		else
			glBindTexture(GL_TEXTURE_2D, textureBlack);

		// Diffuse
		glActiveTexture(GL_TEXTURE2);
		shader->SetUniform1i("material.texture_normal", 2);

		if (textureNormal.id != 0)
			glBindTexture(GL_TEXTURE_2D, textureNormal.id);
		else
			glBindTexture(GL_TEXTURE_2D, textureBlack);

		// Diffuse
		glActiveTexture(GL_TEXTURE3);
		shader->SetUniform1i("material.texture_height", 3);

		if (textureHeight.id != 0)
			glBindTexture(GL_TEXTURE_2D, textureHeight.id);
		else
			glBindTexture(GL_TEXTURE_2D, textureBlack);

		// Diffuse
		glActiveTexture(GL_TEXTURE4);
		shader->SetUniform1i("material.texture_emission", 4);

		if (textureEmission.id != 0)
			glBindTexture(GL_TEXTURE_2D, textureEmission.id);
		else
			glBindTexture(GL_TEXTURE_2D, textureBlack);
	}

	void Material::Init()
	{
		textureBlack = TextureManager::GetTexture("./assets/textures/black.png");
	}
}
