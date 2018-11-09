#pragma once

#include "EVA.hpp"

namespace EVA
{
	class EVA_API FollowTarget : public Component, public IUpdateComponent
	{
	REGISTER_COMPONENT_HPP(FollowTarget)

		std::string m_TargetName;

	public:

		Transform* target = nullptr;
		glm::vec3 offset = glm::vec3(0.0f);
		float scalar = 10.0f;

		void Start() override;

		void Update(float deltaTime) override;

		void Serialize(DataObject& data) override;
	};
}
