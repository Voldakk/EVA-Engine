#include "FollowTarget.hpp"

#include "glm/gtx/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(FollowTarget, "EVA::FollowTarget")

	void FollowTarget::Start()
	{
		const auto go = scene->FindGameObjectByName(m_TargetName);
		if (go != nullptr)
			target = go->transform.get();
	}

	void FollowTarget::Update(const float deltaTime)
	{
		if (target == nullptr)
			return;

		transform->SetOrientation(target->orientation);


		const auto targetPos = target->position + target->LocalToWorld(offset);
		transform->SetPosition(glm::mix(transform->position, targetPos, deltaTime * scalar));
	}

	void FollowTarget::Serialize(DataObject& data)
	{
		data.Serialize("offset", offset);
		data.Serialize("scalar", scalar);
		data.Serialize("m_TargetName", m_TargetName);
	}
}
