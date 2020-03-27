#pragma once

#include "EVA.hpp"
#include "../Bounds.hpp"

namespace EVA
{
	class Collider : public Component
	{

	public:
		
		Bounds bounds;

		~Collider();

		void Awake() override;

		virtual Bounds GetBounds() = 0;

	};
}
