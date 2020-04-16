#include "Water.hpp"

#include "EVA/ResourceManagers.hpp"
#include "../ScopeTimer.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace EVA
{
	void Water::Start()
	{
		if (m_Quadtree == nullptr)
			m_Quadtree = std::make_unique<Quadtree>();

		m_Material = std::make_shared<WaterMaterial>();
		m_Material->SetWater(this);
		m_Material->SetUseInstancing(true);
		m_Material->shader = ShaderManager::LoadShader(m_ShaderPath);

		auto v = GeneratePatch();
		m_Mesh = std::make_shared<QuadtreeMesh>(v);

		Update(0);
	}

	void Water::Update(float deltaTime)
	{
		glm::vec3 targetPos = Application::mainCamera->transform->position;
		if (m_Target != nullptr)
		{
			glm::vec3 targetScale = glm::vec3(1.0f);
			if (m_Target->parent.Get() != nullptr)
				targetScale = m_Target->parent->scale;
			targetPos = m_Target->position / targetScale;
		}
		targetPos -= transform->position;
		m_Quadtree->scale = glm::vec2(transform->scale.x, transform->scale.z);
		m_Quadtree->Update(glm::vec2(targetPos.x, targetPos.z));

		std::vector<NodeData> leafData;
		m_Quadtree->GetLeafData(leafData);

		{
			SCOPE_TIMER("Water::Update - Mesh data");

			m_MeshData.clear();
			m_MeshData.reserve(leafData.size());
			for (const auto data : leafData)
			{
				auto localMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(data.bounds.GetMin().x, 0.0f, data.bounds.GetMin().y));
				localMatrix = glm::scale(localMatrix, glm::vec3(data.bounds.GetSize().x, 0.0f, data.bounds.GetSize().y));
				m_MeshData.push_back({ transform->modelMatrix, localMatrix, data.tScaleNegX, data.tScalePosX, data.tScaleNegY, data.tScalePosY });
			}
		}
	}

	void Water::Render()
	{
		if (m_Material == nullptr || m_Material->shader == nullptr || m_Mesh == nullptr)
			return;

		SCOPE_TIMER("Water::Render");
		m_Material->Activate(scene.Get(), transform.Get());
		m_Mesh->Draw(m_MeshData);
	}

	void Water::Serialize(DataObject& data)
	{
		data.Serialize("Wave speed", m_WaveSpeed);

		if (data.Serialize("Target", m_TargetName))
		{
			const auto go = scene->FindGameObjectByName(m_TargetName);
			if (go != nullptr)
			{
				m_Target = go->transform.get();
			}
			else
			{
				m_TargetName = "";
				m_Target = nullptr;
			}
		}

		if (data.Serialize("shader", m_ShaderPath))
		{
			if (m_Material != nullptr)
				m_Material->shader = ShaderManager::LoadShader(m_ShaderPath);
		}

		if (m_Quadtree == nullptr)
			m_Quadtree = std::make_unique<Quadtree>();

		data.Serialize("Lod distances", m_Quadtree->lodDistances);

		if (m_Noise == nullptr)
			m_Noise = std::make_shared<Noise>();
		data.Serialize("Noise", m_Noise);
	}

	std::vector<glm::vec2> Water::GeneratePatch()
	{
		std::vector<glm::vec2> vertices(4);

		int index = 0;

		vertices[index++] = glm::vec2(0, 0);
		vertices[index++] = glm::vec2(1, 0);
		vertices[index++] = glm::vec2(0, 1);
		vertices[index++] = glm::vec2(1, 1);

		return vertices;
	}
}