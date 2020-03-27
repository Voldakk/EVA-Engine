#pragma once

#include "EVA.hpp"
#include "EVA/Components.hpp"

#include "Quadtree.hpp"
#include "TerrainMaterial.hpp"
#include "TerrainMesh.hpp"

namespace EVA
{

	class Terrain : public Component, public ILateUpdateComponent, public IRenderComponent
	{
		REGISTER_COMPONENT(Terrain, "EVA::Terrain");

		std::unique_ptr<Quadtree> m_Quadtree;

		std::shared_ptr<TerrainMaterial> m_Material = nullptr;
		std::shared_ptr<TerrainMesh> m_Mesh = nullptr;

		std::vector<TerrainMeshData> m_MeshData;

		std::vector<int> m_LodDistances = std::vector<int>(8);

		std::string m_TargetName;
		Transform* m_Target;

	public:

		const std::vector<int>& lodDistances = m_LodDistances;		

		void Start() override;
		void LateUpdate() override;

		void Render() override;

		virtual void Serialize(DataObject& data) override;

		static std::vector<glm::vec2> GeneratePatch();
	};

}
