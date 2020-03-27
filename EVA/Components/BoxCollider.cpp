#include "BoxCollider.hpp"

namespace EVA
{
	BoxCollider::BoxCollider(const Bounds bounds)
	{
		this->bounds = bounds;
	}

	Bounds BoxCollider::GetBounds()
	{
		return Bounds(bounds.GetCenter() + transform->position, bounds.GetExtents() * transform->scale);
	}

	void BoxCollider::Serialize(DataObject& data)
	{
		//data.Serialize("center", bounds.center);
		//data.Serialize("extents", bounds.extents);
	}
}
