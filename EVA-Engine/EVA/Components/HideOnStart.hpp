#pragma once

#include "EVA.hpp"

namespace EVA
{
	class EVA_API HideOnStart : public Component, public IUpdateComponent
	{
	REGISTER_COMPONENT_HPP(HideOnStart);

	public:
		void Update(float deltaTime) override;
	};
}
