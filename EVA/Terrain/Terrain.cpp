#include "Terrain.hpp"

#include "EVA/ResourceManagers.hpp"
#include "TerrainMaterial.hpp"

#include "glm/glm.hpp"
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtx\quaternion.hpp>

#include "../ScopeTimer.hpp"

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

		m_Quadtree = std::make_unique<Quadtree>(this, glm::vec2(0, 0), Bounds2::MinMax(glm::vec2(0.0f), glm::vec2(1.0f)));

		const auto go = scene->FindGameObjectByName(m_TargetName);
		if (go != nullptr)
			m_Target = go->transform.get();

		LateUpdate();
	}

	void Terrain::LateUpdate()
	{
		if (m_Material == nullptr)
			return;

		{
			ScopeTimer timer("Quadtree Update"); // 1.1 ms

			glm::vec3 targetPos = Application::mainCamera->transform->position;
			if (m_Target != nullptr)
			{
				glm::vec3 targetScale = glm::vec3(1.0f);
				if (m_Target->parent.Get() != nullptr)
					targetScale = m_Target->parent->scale;
				targetPos = m_Target->position / targetScale;
			}
			targetPos -= transform->position;

			m_Quadtree->Update(targetPos);
		}

		std::vector<NodeData> leafData;

		{
			ScopeTimer timer("Quadtree GetLeafData"); // 1.3 ms

			m_Quadtree->GetLeafData(leafData);
		}

		{
			ScopeTimer timer("Terrain update mesh data"); // 3.5 ms

			m_MeshData.clear();
			m_MeshData.reserve(leafData.size());
			for (const auto data : leafData)
			{
				auto localMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(data.bounds.GetMin().x, 0.0f, data.bounds.GetMin().y));
				localMatrix = glm::scale(localMatrix, glm::vec3(data.bounds.GetSize().x, 0.0f, data.bounds.GetSize().y));
				m_MeshData.push_back({ transform->modelMatrix, localMatrix });
			}
		}
	}

	void Terrain::Render()
	{
		if (m_Material == nullptr)
			return;

		{
			ScopeTimer timer("Terrain Render");
			m_Material->Activate(scene.Get(), nullptr);
			m_Mesh->DrawTerrain(m_MeshData);
		}
	}

	void Terrain::Serialize(DataObject& data)
	{
		auto path = m_Heightmap != nullptr ? m_Heightmap->path : "";
		if (data.Serialize("Heightmap", path))
		{
			m_Heightmap = TextureManager::LoadTexture(path, TextureWrapping::ClampToEdge, TextureMinFilter::Linear, TextureMagFilter::Linear);
		}

		data.Serialize("Tesselation factor", m_TessFactor);
		data.Serialize("Tesselation slope", m_TessSlope);
		data.Serialize("Tesselation shift", m_TessShift);

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

		data.Serialize("Lod distances", m_LodDistances);
	}

	std::vector<glm::vec2> Terrain::GeneratePatch()
	{
		std::vector<glm::vec2> vertices(16);

		int index = 0;

		vertices[index++] = glm::vec2(0, 0);
		vertices[index++] = glm::vec2(0.333f, 0);
		vertices[index++] = glm::vec2(0.666f, 0);
		vertices[index++] = glm::vec2(1, 0);

		vertices[index++] = glm::vec2(0, 0.333f);
		vertices[index++] = glm::vec2(0.333f, 0.333f);
		vertices[index++] = glm::vec2(0.666f, 0.333f);
		vertices[index++] = glm::vec2(1, 0.333f);

		vertices[index++] = glm::vec2(0, 0.666f);
		vertices[index++] = glm::vec2(0.333f, 0.666f);
		vertices[index++] = glm::vec2(0.666f, 0.666f);
		vertices[index++] = glm::vec2(1, 0.666f);

		vertices[index++] = glm::vec2(0, 1);
		vertices[index++] = glm::vec2(0.333f, 1);
		vertices[index++] = glm::vec2(0.666f, 1);
		vertices[index++] = glm::vec2(1, 1);

		return vertices;
	}
}
