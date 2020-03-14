#pragma once

#include "Collider.hpp"

namespace EVA
{

	class BoxCollider : public Collider
	{
		REGISTER_COMPONENT(BoxCollider, "EVA::BoxCollider")

	public:


		BoxCollider() = default;
		explicit BoxCollider(Bounds bounds);
		
		Bounds GetBounds() override;

		virtual void Serialize(DataObject& data) override;
	};

}