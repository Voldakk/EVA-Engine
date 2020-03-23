#include "Terrain.hpp"

#include "EVA/ResourceManagers.hpp"
#include "TerrainMaterial.hpp"

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

		m_Quadtree = std::make_unique<Quadtree>(this, m_Extents);

		LateUpdate();
	}

	void Terrain::LateUpdate()
	{
		if (m_Material == nullptr)
			return;

		m_Quadtree->Update(Application::mainCamera->transform->position);

		std::vector<NodeData> leafData;
		m_Quadtree->GetLeafData(leafData);

		const auto position = transform->localPosition;
		const auto scale = transform->localScale;

		meshData.clear();
		for (const auto data : leafData)
		{
			transform->SetPosition(data.bounds.center);
			transform->SetScale(data.bounds.extents);
			const auto modelMatrix = transform->modelMatrix;

			meshData.push_back({ modelMatrix, (float)data.lod, glm::vec4(1.0f) });
		}

		transform->SetPosition(position);
		transform->SetScale(scale);
	}

	void Terrain::Render()
	{
		if (m_Material == nullptr)
			return;

		m_Material->Activate(scene.Get(), nullptr);
		m_Mesh->DrawTerrain(meshData);
	}

	void Terrain::Serialize(DataObject& data)
	{
		data.Serialize("Lod distances", m_LodDistances);
	}

	std::vector<glm::vec3> Terrain::GeneratePatch()
	{
		std::vector<glm::vec3> vertices(4);

		vertices[0] = { -1.0f, 0.0f, -1.0f };
		vertices[1] = { 1.0f, 0.0f, -1.0f };
		vertices[2] = { -1.0f, 0.0f,  1.0f };
		vertices[3] = { 1.0f, 0.0f,  1.0f };

		return vertices;
	}
}
