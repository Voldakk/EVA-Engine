#include "MeshRenderer.hpp"

#include "../Scene.hpp"
#include "../ModelManager.hpp"
#include "../MaterialManager.hpp"

namespace EVA
{
	MeshRenderer::~MeshRenderer()
	{
		scene->RemoveMeshRenderer(this);
	}

	void MeshRenderer::Start()
	{
		
	}

	void MeshRenderer::Set(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material)
	{
		if (m_Mesh != nullptr || m_Material != nullptr)
			scene->RemoveMeshRenderer(this);

		m_Mesh = mesh;
		m_Material = material;

		if (mesh != nullptr)
		{
			m_ModelPath = m_Mesh->path;
			m_MeshIndex = m_Mesh->index;
		}
		
		if (m_Mesh == nullptr || m_Material == nullptr)
			return;

		scene->RegisterMeshRenderer(this);
	}

	void MeshRenderer::Render() const
	{
		m_Material->Activate(scene.Get(), transform.Get());
		m_Mesh->Draw();
	}

	void MeshRenderer::Serialize(DataObject& data)
	{
		auto newMaterial = m_Material;

		// Material
		auto materialPath = m_Material != nullptr ? m_Material->path : "";
		if (data.Serialize("material", materialPath))
		{
			newMaterial = MaterialManager::LoadMaterial(materialPath);
		}

		// Model
		data.Serialize("mesh", m_ModelPath);

		int meshIndex = m_MeshIndex;
		data.Serialize("meshIndex", meshIndex);

		if (data.changed)
		{
			auto newMesh = GetMesh(m_ModelPath, meshIndex);
			Set(newMesh, newMaterial);
		}
	}

	std::shared_ptr<Mesh> MeshRenderer::GetMesh(const FS::path& modelPath, const unsigned meshIndex)
	{
		m_MeshIndex = meshIndex;
		m_ModelPath = modelPath;

		if (!modelPath.empty())
		{
			const auto model = ModelManager::LoadModel(modelPath);
			if (model == nullptr)
			{
				m_ModelPath = "";
				return nullptr;
			}
			else if (model->MeshCount() > meshIndex)
			{
				return model->GetMesh(meshIndex);
			}
		}

		return nullptr;
	}
}
