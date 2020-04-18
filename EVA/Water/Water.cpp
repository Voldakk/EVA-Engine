#include "Water.hpp"

#include "EVA/ResourceManagers.hpp"
#include "../ScopeTimer.hpp"

#include "glm/gtc/quaternion.hpp"

namespace EVA
{
	void Water::Start()
	{
		if (m_Quadtree == nullptr)
			m_Quadtree = std::make_shared<Quadtree>();

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

			m_Mesh->data.clear();
			m_Mesh->data.reserve(leafData.size());
			for (const auto data : leafData)
			{
				m_Mesh->data.push_back({ data.localMatrix, data.tScaleNegX, data.tScalePosX, data.tScaleNegY, data.tScalePosY });
			}
		}
	}

	void Water::Render()
	{
		if (m_Material == nullptr || m_Material->shader == nullptr || m_Mesh == nullptr)
			return;

		SCOPE_TIMER("Water::Render");
		m_Material->Activate(scene.Get(), transform.Get());
		m_Mesh->Draw();
	}

	void Water::Serialize(DataObject& data)
	{
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
			m_Quadtree = std::make_shared<Quadtree>();
		data.Serialize("Quadtree", m_Quadtree);

		if (m_Noise == nullptr)
			m_Noise = std::make_shared<Noise>();
		bool recalculateSpeeds = data.Serialize("Noise", m_Noise);

		recalculateSpeeds = data.Serialize("Wave speed", m_WaveSpeed) || recalculateSpeeds;

		if (recalculateSpeeds)
		{
			m_WaveSpeeds.resize(m_Noise->octaveOffsets.size());
			glm::quat rot(glm::vec3(m_WaveSpeed.x, m_WaveSpeed.y, 0));

			std::random_device r;
			std::default_random_engine dre(r());
			std::uniform_real_distribution<float> uniformDist(-0.5, 0.5);

			glm::vec3 ws = glm::vec3(m_WaveSpeed.x, m_WaveSpeed.y, 0);
			for (size_t i = 0; i < m_WaveSpeeds.size(); i++)
			{
				glm::vec3 speed = glm::angleAxis(uniformDist(dre) * i, ZAXIS) * ws * (i + 1.0f);
				m_WaveSpeeds[i] = glm::vec2(speed.x, speed.y);
			}
		}

		if (data.mode == DataObject::DataMode::Inspector)
		{
			for (int i = 0; i < m_WaveSpeeds.size(); i++)
			{
				InspectorFields::Text("Wave speed " + std::to_string(i) + ": (" + std::to_string(m_WaveSpeeds[i].x) + ", " + std::to_string(m_WaveSpeeds[i].y) + ")");
			}
		}
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