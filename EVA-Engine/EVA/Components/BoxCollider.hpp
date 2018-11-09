#pragma once

#include "Collider.hpp"

namespace EVA
{
	class EVA_API BoxCollider : public Collider
	{
	REGISTER_COMPONENT_HPP(BoxCollider)

	public:


		BoxCollider() = default;
		explicit BoxCollider(Bounds bounds);

		Bounds GetBounds() override;

		void Serialize(DataObject& data) override;
	};
}
