#pragma once

#include "EVA.hpp"

#include "Camera.hpp"
#include "../Input.hpp"

namespace EVA
{

	class FreeLook : public Component, public IUpdateComponent
	{
		REGISTER_COMPONENT_HPP(FreeLook)

		Camera* m_Camera = nullptr;

	public:

		enum Action { Forward, Back, Right, Left, Up, Down };
		std::map<Action, int> keyBindings = 
		{ 
			{ Forward, Input::W },
			{ Back,    Input::S },
			{ Right,   Input::D },
			{ Left,    Input::A },
			{ Up,      Input::Space },
			{ Down,    Input::LeftShift }
		};

		float mouseSensitivity = 50.0f;
		float minMovementSpeed = 10.0f;
		float currentMovementSpeed = minMovementSpeed;

		float pitch = 0.0f;
		float yaw = 0.0f;
		
		void Start() override;

		void Update(const float deltaTime) override;

		/**
		* \brief Loads Component values from the given DataObject
		* \param data The DataObject
		*/
		void LoadAsset(DataObject data) override;

		/**
		* \brief Saves Component values to the given DataObject
		* \param data The DataObject
		*/
		void SaveAsset(DataObject& data) const override;

		/**
		* \brief Draws the component inspector
		*/
		void DrawInspector() override;
	};
}
