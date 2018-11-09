#pragma once

#include "../Component.hpp"
#include "../ComponentMap.hpp"

namespace EVA
{
	/**
	* \brief A light. Used to light up a Scene
	*/
	class EVA_API Light : public Component
	{
	REGISTER_COMPONENT_HPP(Light)

	public:

		enum Type
		{
			Directional,
			Point
		};

	private:

		Type m_Type = Directional;

		// Shadows
		bool m_Shadows = false;
		unsigned int m_DepthMapFb{};
		unsigned int m_DepthMap{};
		unsigned int m_ShadowMapSize = 2048;

	public:

		float directionalShadowDistance = 20.0f;

		float directionalNearPlane = 1.0f;
		float directionalFarPlane = 100.0f;

		float pointNearPlane = 0.1f;
		float pointFarPlane = 25.0f;

		// Common
		glm::vec3 color = glm::vec3(1.0f);
		float ambientCoefficient = 0.005f;

		// Point light
		float attenuation = 0.2f;

		~Light();

		void Awake() override;

		// Get stuff
		Type GetType() const { return m_Type; }

		glm::mat4 GetLightSpaceMatrix() const;
		bool Shadows() const { return m_Shadows; }
		unsigned int GetDepthMapFb() const { return m_DepthMapFb; }
		unsigned int GetDepthMap() const { return m_DepthMap; }
		unsigned int GetShadwoSize() const { return m_ShadowMapSize; }

		std::vector<glm::mat4> GetShadowTransforms() const;

		void Serialize(DataObject& data) override;

		void GenerateTexturesaAndFrameBuffer();
	};
}
