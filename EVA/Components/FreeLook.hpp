#pragma once

#include "EVA.hpp"

#include "Camera.hpp"
#include "../Input.hpp"

namespace EVA
{

	class FreeLook : public Component, public IUpdateComponent
	{
		REGISTER_COMPONENT(FreeLook, "EVA::FreeLook")

		Camera* m_Camera = nullptr;

	public:

		enum class Action { Forward, Back, Right, Left, Up, Down, ToggleCursor };
		std::map<Action, Input::Key> keyBindings =
		{ 
			{ Action::Forward, Input::Key::W },
			{ Action::Back,    Input::Key::S },
			{ Action::Right,   Input::Key::D },
			{ Action::Left,    Input::Key::A },
			{ Action::Up,      Input::Key::Space },
			{ Action::Down,    Input::Key::LeftShift },
			{ Action::ToggleCursor,    Input::Key::Escape }

		};

		float mouseSensitivity = 50.0f;
		float minMovementSpeed = 10.0f;
		float currentMovementSpeed = minMovementSpeed;

		float pitch = 0.0f;
		float yaw = 0.0f;
		
		void Start() override;

		void Update(const float deltaTime) override;

		virtual void Serialize(DataObject& data) override;
	};
}
