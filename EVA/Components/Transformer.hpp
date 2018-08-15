#pragma once

#include "EVA.hpp"

namespace EVA
{

	class Transformer : public Component, public IUpdateComponent
	{
		REGISTER_COMPONENT_HPP(Transformer)

		glm::vec3 m_Translation;
		glm::vec3 m_Scaling;
		glm::vec3 m_Rotation;

	public:
		
		void Update(float deltaTime) override;

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