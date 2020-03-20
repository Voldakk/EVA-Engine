#pragma once

#include "FreeLook.hpp"

#include "EVA/Input.hpp"

namespace EVA
{
	
	class SceneCamera : public FreeLook
	{

	public:
		
		void Update(float deltaTime) override;
	};

	inline void SceneCamera::Update(const float deltaTime)
	{
		if (Input::GetMouseButton(Input::Mouse::MouseRight))
		{
			Input::SetCursorMode(Input::CursorMode::Disabled);
			FreeLook::Update(deltaTime);
		}
		else
		{
			Input::SetCursorMode(Input::CursorMode::Normal);
		}
	}
}
