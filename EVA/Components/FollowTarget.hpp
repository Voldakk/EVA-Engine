#pragma once

#include "EVA.hpp"

namespace EVA
{

	class FollowTarget : public Component, public IUpdateComponent
	{
		REGISTER_COMPONENT(FollowTarget, "EVA::FollowTarget")

		std::string m_TargetName;

	public:

		Transform * target = nullptr;
		glm::vec3 offset = glm::vec3(0.0f);
		float scalar = 10.0f;

		void Start() override;

		void Update(const float deltaTime) override;

		virtual void Serialize(DataObject& data) override;
	};
}