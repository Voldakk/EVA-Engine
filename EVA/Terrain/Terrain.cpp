#include "Terrain.hpp"

#include "EVA/ResourceManagers.hpp"
#include "TerrainMaterial.hpp"
#include "NormalMapRenderer.hpp"
#include "SpatMapRenderer.hpp"
#include "../ScopeTimer.hpp"

#include "glm/glm.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

#include "../Parsers/Json.hpp"


namespace EVA
{
	void Terrain::Start()
	{
		if (m_Quadtree == nullptr)
			m_Quadtree = std::make_shared<Quadtree>();

		m_Material = std::make_shared<TerrainMaterial>();
		m_Material->SetTerrain(this);
		m_Material->SetUseInstancing(true);
		m_Material->shader = ShaderManager::LoadShader(m_ShaderPath);

		auto v = GeneratePatch();
		m_Mesh = std::make_shared<QuadtreeMesh>(v);

		const auto go = scene->FindGameObjectByName(m_TargetName);
		if (go != nullptr)
			m_Target = go->transform.get();

		Update(0);
	}

	void Terrain::Update(float deltaTime)
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
			SCOPE_TIMER("Terrain::Update - Mesh data");

			m_Mesh->data.clear();
			m_Mesh->data.reserve(leafData.size());
			for (const auto data : leafData)
			{
				m_Mesh->data.push_back({ data.localMatrix, data.tScaleNegX, data.tScalePosX, data.tScaleNegY, data.tScalePosY });
			}
		}
	}

	void Terrain::Render()
	{
		if (m_Material == nullptr || m_Material->shader == nullptr || m_Mesh == nullptr)
			return;

		SCOPE_TIMER("Terrain Render");
		m_Material->Activate(scene.Get(), transform.Get());
		m_Mesh->Draw();
	}

	void Terrain::Serialize(DataObject& data)
	{
		std::cout << "Terrain::Serialize" << std::endl;

		data.Serialize("TBN range", m_TbnRange);

		bool recalculateNormals = data.Serialize("Normal strength", m_NormalStrength);

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
			if(m_Material != nullptr)
				m_Material->shader = ShaderManager::LoadShader(m_ShaderPath);
		}

		auto path = m_Heightmap != nullptr ? m_Heightmap->path : "";
		if (data.Serialize("Heightmap", path))
		{
			m_Heightmap = TextureManager::LoadTexture(path, TextureWrapping::ClampToEdge, TextureMinFilter::Linear, TextureMagFilter::Linear);
			recalculateNormals = true;
		}


		for (size_t i = 0; i < layers.size(); i++)
		{
			data.Serialize("layer" + std::to_string(i), m_Layers[i]);
		}

		if(m_Quadtree == nullptr)
			m_Quadtree = std::make_shared<Quadtree>();
		data.Serialize("Quadtree", m_Quadtree);

		if ((recalculateNormals && m_Heightmap != nullptr) || (data.mode == DataObject::DataMode::Inspector && InspectorFields::Button("Recalculate")))
		{
			{
				ScopeTimer timer("Terrain generate normalmap");
				NormalMapRenderer nmr;
				m_Normalmap = nmr.Render(m_Heightmap, m_Heightmap->width, m_NormalStrength);
			}
			{
				ScopeTimer timer("Terrain generate splatmap");
				SplatMapRenderer smr;
				m_Splatmaps = smr.Render(m_Normalmap->width, m_Normalmap, m_Heightmap, m_Layers);
			}
		}
	}

	std::vector<glm::vec2> Terrain::GeneratePatch()
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
