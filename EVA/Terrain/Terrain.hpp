#pragma once

#include "EVA.hpp"
#include "EVA/Components.hpp"


#include "Quadtree.hpp"

namespace EVA
{

	class Terrain : public Component, public ILateUpdateComponent, public IRenderComponent
	{
		REGISTER_COMPONENT_HPP(Terrain);

		glm::vec3 m_Extents = glm::vec3(100.0f, 1.0f, 100.0f);
		std::unique_ptr<Quadtree> m_Quadtree;

		std::shared_ptr<Material> m_Material = nullptr;
		std::shared_ptr<Mesh> m_Mesh = nullptr;

	public:

		void Start() override;
		void LateUpdate() override;

		void Render() override;

		void Inspector() override;

		std::vector<glm::vec3> GeneratePatch();
	};

}
