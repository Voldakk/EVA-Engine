#pragma once

#include "../Component.hpp"
#include "../ComponentMap.hpp"

namespace EVA
{
	/**
	* \brief A light. Used to light up a Scene
	*/
	class Light : public Component
	{
		REGISTER_COMPONENT(Light, "EVA::Light")

	public:

		enum Type
		{
			Directional, Point
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

		virtual void Awake() override;

		// Get stuff
		inline Type GetType() const { return m_Type; }

		glm::mat4 GetLightSpaceMatrix() const;
		inline bool Shadows() const { return m_Shadows; }
		inline unsigned int GetDepthMapFb() const { return m_DepthMapFb; }
		inline unsigned int GetDepthMap() const { return m_DepthMap; }
		inline unsigned int GetShadwoSize() const { return m_ShadowMapSize; }

		std::vector<glm::mat4> GetShadowTransforms() const;

		virtual void Serialize(DataObject& data) override;

		void GenerateTexturesaAndFrameBuffer();
	};

}
