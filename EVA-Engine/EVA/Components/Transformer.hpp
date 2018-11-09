#pragma once

#include "EVA.hpp"

namespace EVA
{
	class EVA_API Transformer : public Component, public IUpdateComponent
	{
	REGISTER_COMPONENT_HPP(Transformer)

		glm::vec3 m_Translation;
		glm::vec3 m_Scaling;
		glm::vec3 m_Rotation;

	public:

		void Update(float deltaTime) override;

		void Serialize(DataObject& data) override;
	};
}
