#include "Terrain.hpp"

#include "EVA/ResourceManagers.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(Terrain, "Terrain")

	void Terrain::Start()
	{
		m_Quadtree = std::make_unique<Quadtree>(m_Extents);

		m_MeshRenderer = gameObject->GetComponentOfType<MeshRenderer>();

		m_Quadtree->Subdivide();

		m_Quadtree->northEast->Subdivide();

		LateUpdate();
	}

	void Terrain::LateUpdate()
	{
		if(m_MeshRenderer == nullptr)
			return;

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

		m_MeshRenderer->material->SetMbo(m_MeshRenderer->mesh, modelMatrices);
	}

	void Terrain::Inspector()
	{
		if (ComponentInspector::Button("LateUpdate"))
			LateUpdate();
	}

}
