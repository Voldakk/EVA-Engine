#pragma once

#include "EVA.hpp"

namespace EVA
{

	class Transformer : public Component, public IUpdateComponent
	{
		REGISTER_COMPONENT(Transformer, "EVA::Transformer")

		glm::vec3 m_Translation;
		glm::vec3 m_Scaling;
		glm::vec3 m_Rotation;

	public:
		
		void Update(float deltaTime) override;

		virtual void Serialize(DataObject& data) override;
	};

}