#pragma once

#include "FreeLook.hpp"

#include "EVA/Input.hpp"

namespace EVA
{
	class EVA_API SceneCamera : public FreeLook
	{
	public:

		void Update(float deltaTime) override;
	};

	inline void SceneCamera::Update(const float deltaTime)
	{
		if (Input::MouseButton(Input::Button::MouseRight))
		{
			Input::SetCursorMode(Input::Disabled);
			FreeLook::Update(deltaTime);
		}
		else
		{
			Input::SetCursorMode(Input::Normal);
		}
	}
}
