#include "Terrain.hpp"

#include "EVA/ResourceManagers.hpp"
#include "TerrainMaterial.hpp"

#include "glm/glm.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\quaternion.hpp>

namespace EVA
{
	void Terrain::Start()
	{
		m_Material = std::make_shared<TerrainMaterial>();
		m_Material->SetTerrain(this);
		m_Material->shader = ShaderManager::LoadShader("./assets/standard assets/shaders/terrain.shader");
		m_Material->SetUseInstancing(true);

		auto v = GeneratePatch();
		m_Mesh = std::make_shared<TerrainMesh>(v);

		m_Quadtree = std::make_unique<Quadtree>(this, glm::vec2(0, 0), Bounds2::MinMax(glm::vec2(0.0f), glm::vec2(100.0f)));

		const auto go = scene->FindGameObjectByName(m_TargetName);
		if (go != nullptr)
			m_Target = go->transform.get();

		LateUpdate();
	}

	void Terrain::LateUpdate()
	{
		if (m_Material == nullptr)
			return;

		glm::vec3 targetPos = Application::mainCamera->transform->position;
		if (m_Target != nullptr)
		{
			glm::vec3 targetScale = glm::vec3(1.0f);
			if (m_Target->parent.Get() != nullptr)
				targetScale = m_Target->parent->scale;
			targetPos = m_Target->position / targetScale;
		}

		m_Quadtree->Update(targetPos - transform->position);

		std::vector<NodeData> leafData;
		m_Quadtree->GetLeafData(leafData);

		m_MeshData.clear();
		for (const auto data : leafData)
		{
			auto modelMatrix = glm::translate(transform->modelMatrix, glm::vec3(data.bounds.GetMin().x, 1.0f, data.bounds.GetMin().y));
			modelMatrix = glm::scale(modelMatrix, glm::vec3(data.bounds.GetSize().x, 1.0f, data.bounds.GetSize().y));
			m_MeshData.push_back({ modelMatrix, data.lod, data.index });
		}
	}

	void Terrain::Render()
	{
		if (m_Material == nullptr)
			return;

		m_Material->Activate(scene.Get(), nullptr);
		m_Mesh->DrawTerrain(m_MeshData);
	}

	void Terrain::Serialize(DataObject& data)
	{
		if (data.Serialize("m_TargetName", m_TargetName))
		{
			const auto go = scene->FindGameObjectByName(m_TargetName);
			if (go != nullptr)
				m_Target = go->transform.get();
		}
		data.Serialize("Lod distances", m_LodDistances);
	}

	std::vector<glm::vec2> Terrain::GeneratePatch()
	{
		std::vector<glm::vec2> vertices(4);

		vertices[0] = { 0.0f, 0.0f };
		vertices[1] = { 1.0f, 0.0f };
		vertices[2] = { 0.0f, 1.0f };
		vertices[3] = { 1.0f, 1.0f };

		return vertices;
	}
}
