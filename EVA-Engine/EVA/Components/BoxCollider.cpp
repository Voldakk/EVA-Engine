#include "BoxCollider.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(BoxCollider, "EVA::BoxCollider")

	BoxCollider::BoxCollider(const Bounds bounds)
	{
		this->bounds = bounds;
	}

	Bounds BoxCollider::GetBounds()
	{
		return Bounds(bounds.center + transform->position, bounds.extents * transform->scale);
	}

	void BoxCollider::Serialize(DataObject& data)
	{
		data.Serialize("center", bounds.center);
		data.Serialize("extents", bounds.extents);
	}
}
