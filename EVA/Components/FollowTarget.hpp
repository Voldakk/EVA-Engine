#pragma once

#include "EVA.hpp"

namespace EVA
{

	class FollowTarget : public Component, public IUpdateComponent
	{
		REGISTER_COMPONENT_HPP(FollowTarget)

		std::string m_TargetName;

	public:

		Transform * target = nullptr;
		glm::vec3 offset = glm::vec3(0.0f);
		float scalar = 10.0f;

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