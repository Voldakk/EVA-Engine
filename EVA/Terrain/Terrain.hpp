#pragma once

#include "EVA.hpp"
#include "EVA/Components.hpp"

#include "TerrainMaterial.hpp"
#include "TerrainLayer.hpp"
#include "../Quadtree/QuadtreeMesh.hpp"
#include "../Quadtree/Quadtree.hpp"

namespace EVA
{
	class Terrain : public Component, public IUpdateComponent, public IRenderComponent
	{
		REGISTER_COMPONENT(Terrain, "EVA::Terrain");

		std::shared_ptr<Quadtree> m_Quadtree;

		std::shared_ptr<TerrainMaterial> m_Material = nullptr;
		FS::path m_ShaderPath;
		std::shared_ptr<QuadtreeMesh> m_Mesh = nullptr;

		std::shared_ptr<Texture> m_Heightmap;
		std::shared_ptr<Texture> m_Normalmap;
		std::vector<std::shared_ptr<Texture>> m_Splatmaps;

		std::string m_TargetName;
		Transform* m_Target;

		std::vector<std::shared_ptr<TerrainLayer>> m_Layers = std::vector<std::shared_ptr<TerrainLayer>>(5);

		int m_TbnRange = 200;
		int m_NormalStrength = 10;

	public:

		const std::shared_ptr<Texture>& heightmap = m_Heightmap;
		const std::shared_ptr<Texture>& normalmap = m_Normalmap;
		const std::vector<std::shared_ptr<Texture>>& splatmaps = m_Splatmaps;
		const std::vector<std::shared_ptr<TerrainLayer>>& layers = m_Layers;

		const int& tbnRange = m_TbnRange;

		void Start() override;
		void Update(float deltaTime) override;

		void Render() override;

		virtual void Serialize(DataObject& data) override;

		static std::vector<glm::vec2> GeneratePatch();
	};

}
