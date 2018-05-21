#pragma once

#include "EVA.hpp"
#include "EVA/Components.hpp"

#include "Quadtree.hpp"
#include "TerrainMaterial.hpp"

namespace EVA
{

	class Terrain : public Component, public ILateUpdateComponent, public IRenderComponent
	{
		REGISTER_COMPONENT_HPP(Terrain);

		glm::vec3 m_Extents = glm::vec3(100.0f, 1.0f, 100.0f);
		std::unique_ptr<Quadtree> m_Quadtree;

		std::shared_ptr<TerrainMaterial> m_Material = nullptr;
		std::shared_ptr<Mesh> m_Mesh = nullptr;

	public:

		int tessLevelOuter = 1;
		int tessLevelInner0 = 1;
		int tessLevelInner1 = 1;

		void Start() override;
		void LateUpdate() override;

		void Render() override;

		void Inspector() override;

		static std::vector<glm::vec3> GeneratePatch();
	};

}
