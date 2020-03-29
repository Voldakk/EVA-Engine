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

		std::shared_ptr<Texture> m_Heightmap;
		std::shared_ptr<Texture> m_Normalmap;

		std::string m_TargetName;
		Transform* m_Target;

		std::vector<int> m_LodDistances = std::vector<int>(8);

		float m_Scale = 500;

		float m_TessFactor = 500;
		float m_TessSlope = 2.0f;
		float m_TessShift = 0.1f;

	public:

		const std::shared_ptr<Texture>& heightmap = m_Heightmap;
		const std::shared_ptr<Texture>& normalmap = m_Normalmap;

		const std::vector<int>& lodDistances = m_LodDistances;

		const float& tessFactor = m_TessFactor;
		const float& tessSlope = m_TessSlope;
		const float& tessShift = m_TessShift;

		void Start() override;
		void LateUpdate() override;

		void Render() override;

		virtual void Serialize(DataObject& data) override;

		static std::vector<glm::vec2> GeneratePatch();
	};

}