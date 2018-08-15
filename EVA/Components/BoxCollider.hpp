#pragma once

#include "Collider.hpp"

namespace EVA
{

	class BoxCollider : public Collider
	{
		REGISTER_COMPONENT_HPP(BoxCollider)

	public:


		BoxCollider() = default;
		explicit BoxCollider(Bounds bounds);
		
		Bounds GetBounds() override;

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