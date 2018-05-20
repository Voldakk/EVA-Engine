#pragma once

#include "EVA.hpp"
#include "EVA/Components.hpp"


#include "Quadtree.hpp"

namespace EVA
{

	class Terrain : public Component, public ILateUpdateComponent
	{
		REGISTER_COMPONENT_HPP(Terrain);

		glm::vec3 m_Extents = glm::vec3(10.0f, 1.0f, 10.0f);
		std::unique_ptr<Quadtree> m_Quadtree;

		MeshRenderer* m_MeshRenderer = nullptr;

	public:

		void Start() override;
		void LateUpdate() override;

		void Inspector() override;
	};

}
