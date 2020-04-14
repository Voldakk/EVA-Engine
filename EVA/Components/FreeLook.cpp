#include "FreeLook.hpp"

#include "EVA/Input.hpp"
#include "Transformer.hpp"
#include "Transform.hpp"

namespace EVA
{
	void FreeLook::Start()
	{
		m_Camera = gameObject->GetComponentOfType<Camera>();
		Input::SetCursorMode(Input::CursorMode::Disabled);
	}

	void FreeLook::Update(const float deltaTime)
	{
		if (Input::GetKeyDown(keyBindings[Action::ToggleCursor]))
		{
			if (Input::GetCursorMode() == Input::CursorMode::Disabled)
				Input::SetCursorMode(Input::CursorMode::Normal);
			else
				Input::SetCursorMode(Input::CursorMode::Disabled);
		}

		if (Input::GetCursorMode() != Input::CursorMode::Disabled)
			return;

		if (Input::GetKey(keyBindings[Action::Forward]))
			currentMovementSpeed *= 1.0f + deltaTime;
		if (Input::GetKeyUp(keyBindings[Action::Forward]))
			currentMovementSpeed = minMovementSpeed;

		// Movement
		glm::vec3 movement;

		// Front
		if (Input::GetKey(keyBindings[Action::Forward]))
			movement += transform->forward;

		// Back
		if (Input::GetKey(keyBindings[Action::Back]))
			movement -= transform->forward;

		// Right
		if (Input::GetKey(keyBindings[Action::Right]))
			movement += transform->right;

		// Left
		if (Input::GetKey(keyBindings[Action::Left]))
			movement -= transform->right;

		// Up
		if (Input::GetKey(keyBindings[Action::Up]))
			movement += transform->up;

		// Down
		if (Input::GetKey(keyBindings[Action::Down]))
			movement -= transform->up;

		transform->Translate(movement * currentMovementSpeed * deltaTime);

		// Look
		const auto mouseMovement = Input::MouseMovement();
		pitch -= mouseMovement.y * mouseSensitivity * deltaTime;
		yaw -= mouseMovement.x * mouseSensitivity * deltaTime;

		// Clamp
		pitch = glm::clamp(pitch, -89.0f, 89.0f);
		if (yaw < 0.0f)
			yaw += 360.0f;
		else if (yaw > 360.0f)
			yaw -= 360.0f;

		transform->SetOrientation(YAXIS, yaw);
		transform->Rotate(transform->right, pitch);

		if (m_Camera != nullptr)
			m_Camera->fov -= Input::GetScroll().y;
	}

	void FreeLook::Serialize(DataObject& data)
	{
		data.Serialize("mouseSensitivity", mouseSensitivity);
		if(data.Serialize("movementSpeed", minMovementSpeed))
			currentMovementSpeed = minMovementSpeed;

		data.Serialize("pitch", pitch);
		data.Serialize("yaw", yaw);
	}
}
