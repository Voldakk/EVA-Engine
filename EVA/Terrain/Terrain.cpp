#include "Terrain.hpp"

#include "EVA/ResourceManagers.hpp"

#include "TerrainMaterial.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(Terrain, "Terrain")

	void Terrain::Start()
	{
		m_Material = std::make_shared<TerrainMaterial>(this);
		m_Material->shader = ShaderManager::LoadShader("./assets/standard assets/shaders/terrain.shader");
		m_Material->SetUseInstancing(true);

		auto v = GeneratePatch();
		m_Mesh = std::make_shared<Mesh>(v);
		gameObject->GetComponentOfType<MeshRenderer>()->Set(m_Mesh, m_Material);

		m_Quadtree = std::make_unique<Quadtree>(m_Extents, 5);

		LateUpdate();
	}

	void Terrain::LateUpdate()
	{
		if(m_Material == nullptr)
			return;

		m_Quadtree->Update(Application::mainCamera->transform->position);

		std::vector<Bounds> bounds;
		m_Quadtree->GetLeafBounds(bounds);

		std::vector<glm::mat4> modelMatrices;

		const auto position = transform->localPosition;
		const auto scale = transform->localScale;

		for (const auto bound : bounds)
		{
			transform->SetPosition(bound.center);
			transform->SetScale(bound.extents);
			modelMatrices.push_back(transform->modelMatrix);
		}

		transform->SetPosition(position);
		transform->SetScale(scale);

		m_Material->SetMbo(m_Mesh, modelMatrices);
	}

	void Terrain::Render()
	{

	}

	void Terrain::Inspector()
	{
		ComponentInspector::Int("tessLevelOuter", tessLevelOuter);
		ComponentInspector::Int("tessLevelInner0", tessLevelInner0);
		ComponentInspector::Int("tessLevelInner1", tessLevelInner1);
	}

	std::vector<glm::vec3> Terrain::GeneratePatch()
	{
		std::vector<glm::vec3> vertices;
		vertices.resize(4);

		/*vertices[0] = { -1.0f, 0.0f, -1.0f };
		vertices[1] = { -0.333f, 0.0f, -1.0f };
		vertices[2] = { 0.333f, 0.0f, -1.0f };
		vertices[3] = { 1.0f, 0.0f, -1.0f };

		vertices[4] = { -1.0f, 0.0f, -0.333f };
		vertices[5] = { -0.333f, 0.0f, -0.333f };
		vertices[6] = { 0.333f, 0.0f, -0.333f };
		vertices[7] = { 1.0f, 0.0f, -0.333f };

		vertices[8] = { -1.0f, 0.0f, 0.333f };
		vertices[9] = { -0.333f, 0.0f, 0.333f };
		vertices[10] = { 0.333f, 0.0f, 0.333f };
		vertices[11] = { 1.0f, 0.0f, 0.333f };

		vertices[12] = { -1.0f, 0.0f, 1.0f };
		vertices[13] = { -0.333f, 0.0f, 1.0f };
		vertices[14] = { 0.333f, 0.0f, 1.0f };
		vertices[15] = { 1.0f, 0.0f, 1.0f };*/

		vertices[0] = { -1.0f, 0.0f, -1.0f };
		vertices[1] = {  1.0f, 0.0f, -1.0f };
		vertices[2] = { -1.0f, 0.0f,  1.0f };
		vertices[3] = {  1.0f, 0.0f,  1.0f };

		return vertices;
	}
}
