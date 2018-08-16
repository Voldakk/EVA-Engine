#include "Material.hpp"

#include "Scene.hpp"
#include "Application.hpp"
#include "ShaderManager.hpp"
#include "Editor/InspectorFields.hpp"
#include "Parsers/MaterialParser.hpp"

namespace EVA
{
	Material *Material::m_ActiveMaterial;

	void Material::SetMbo(const std::shared_ptr<Mesh>& mesh, const std::vector<glm::mat4>& models)
	{
		if(models.size() == 0)
			return;

		if(m_MatrixBuffers.find(mesh) == m_MatrixBuffers.end())
		{
			m_MatrixBuffers[mesh] = InstancedMeshData();
			m_MatrixBuffers[mesh].instanceCount = models.size();
			m_MatrixBuffers[mesh].matrixBuffer = std::make_unique<VertexBuffer>(&models[0], models.size() * sizeof(glm::mat4));
		}
		else
		{
			m_MatrixBuffers[mesh].instanceCount = models.size();
			m_MatrixBuffers[mesh].matrixBuffer->BufferData(&models[0], models.size() * sizeof(glm::mat4));
		}

		m_MatrixBuffers[mesh].dirty = false;
	}

	bool Material::HasMbo(const std::shared_ptr<Mesh>& mesh) const
	{
		return m_MatrixBuffers.find(mesh) != m_MatrixBuffers.end();
	}

	InstancedMeshData* Material::GetMbo(const std::shared_ptr<Mesh>& mesh)
	{
		if (m_MatrixBuffers.find(mesh) != m_MatrixBuffers.end())
			return &m_MatrixBuffers[mesh];

		return nullptr;
	}

	void Material::SetDirtyAll()
	{
		m_MatrixBuffers.clear();
	}

	void Material::SetDirty(const std::shared_ptr<Mesh>& mesh, const bool value)
	{
		if (m_MatrixBuffers.find(mesh) != m_MatrixBuffers.end())
			m_MatrixBuffers[mesh].dirty = value;
	}

	bool Material::IsDirty(const std::shared_ptr<Mesh>& mesh)
	{
		if (m_MatrixBuffers.find(mesh) != m_MatrixBuffers.end())
			return m_MatrixBuffers[mesh].dirty;

		return false;
	}

	void Material::SetUseInstancing(const bool value)
	{
		if(m_UseInstancing == value)
			return;

		m_UseInstancing = value;

		m_MatrixBuffers.clear();
	}

	void Material::SetNoActive()
	{
		m_ActiveMaterial = nullptr;
	}

	void Material::Activate(Scene *scene, Transform *transform)
	{
		if (shader == nullptr)
			return;

		if (m_ActiveMaterial != this)
		{
			m_ActiveMaterial = this;

			// Shader
			shader->Bind();

			// Culling
			if(cullBack && !cullFront)
				glCullFace(GL_BACK);
			else if (cullFront && !cullBack)
				glCullFace(GL_FRONT);
			else if(cullFront && cullBack)
				glCullFace(GL_FRONT_AND_BACK);

			if (cullFront || cullBack)
				glEnable(GL_CULL_FACE);
			else
				glDisable(GL_CULL_FACE);

			// Set uniforms
			SetMaterialUniforms(scene);
		}

		if (transform != nullptr)
			SetObjectUniforms(transform);
	}

	void Material::SetMaterialUniforms(Scene *scene) const
	{
		// Material
		shader->SetUniform1I("instancing", useInstancing);

		// Camera
		shader->SetUniform3Fv("cameraPosition", Application::mainCamera->transform->position);

		// View and projection
		shader->SetUniformMatrix4Fv("view", Application::mainCamera->viewMatrix);
		shader->SetUniformMatrix4Fv("projection", Application::GetPerspectiveMatrix());

		if (scene == nullptr)
			return;

		// Lights
		auto lights = scene->GetLights();
		shader->SetUniform1I("numLights", lights.size());

		auto shadowNum = 0;

		for (unsigned int i = 0; i < lights.size(); ++i)
		{
			const auto lightNum = "allLights[" + std::to_string(i) + "].";

			shader->SetUniform3Fv(lightNum + "color", lights[i]->color);
			shader->SetUniform1F(lightNum + "ambientCoefficient", lights[i]->ambientCoefficient);

			if(lights[i]->GetType() == Light::Type::Directional)
			{
				shader->SetUniform4Fv(lightNum + "position", lights[i]->GetDirection());

				if (lights[i]->Shadows())
				{
					glActiveTexture(GL_TEXTURE5 + shadowNum);
					glBindTexture(GL_TEXTURE_2D, lights[i]->GetDepthMap());
					shader->SetUniform1I(lightNum + "shadowMap", 5 + shadowNum);
					shader->SetUniformMatrix4Fv(lightNum + "lightSpaceMatrix", lights[i]->GetLightSpaceMatrix());

					shadowNum++;
				}
			}
			else if (lights[i]->GetType() == Light::Type::Point)
			{
				shader->SetUniform4Fv(lightNum + "position", glm::vec4(lights[i]->position, 1.0f));
				shader->SetUniform1F(lightNum + "attenuation", lights[i]->attenuation);

				if (lights[i]->Shadows())
				{
					glActiveTexture(GL_TEXTURE5 + shadowNum);
					glBindTexture(GL_TEXTURE_CUBE_MAP, lights[i]->GetDepthMap());
					shader->SetUniform1I("shadowCubeMap", 5 + shadowNum);
					shader->SetUniform1F(lightNum + "farPlane", lights[i]->pointFarPlane);

					shadowNum++;
				}
				else
				{
					shader->SetUniform1F(lightNum + "farPlane", -1.0f);
				}
			}
		}
	}

	void Material::SetObjectUniforms(Transform *transform) const
	{
		// Position
		shader->SetUniformMatrix4Fv("model", transform->modelMatrix);
	}

	void Material::SaveToFile()
	{
		MaterialParser::Save(this, path);
	}

	void Material::Serialize(DataObject& data)
	{
		// Shader
		auto shaderPath = shader != nullptr ? shader->paths->shader : "";
		if (data.Serialize("shader", shaderPath) && !shaderPath.empty())
			shader = ShaderManager::LoadShader(shaderPath);

		// Instancing
		data.Serialize("useInstancing", m_UseInstancing);

		// Culling 
		data.Serialize("cullFront", cullFront);
		data.Serialize("cullBack", cullBack);
	}
}