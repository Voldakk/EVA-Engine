#include "Light.hpp"

#include "GL/glew.h"
#include "glm/gtc/matrix_transform.hpp"
#include "Application.hpp"
#include "Input.hpp"

EVA::Light::Light(const Type type, const bool shadows, const unsigned int shadowSize)
{
	m_Type = type;
	m_Shadows = shadows;
	m_ShadowMapSize = shadowSize;

	if(!shadows)
		return;;

	if(m_Type == Directional)
	{
		glGenTextures(1, &m_DepthMap);
		glBindTexture(GL_TEXTURE_2D, m_DepthMap);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
			m_ShadowMapSize, m_ShadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

		glGenFramebuffers(1, &m_DepthMapFb);
		glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFb);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		glGenTextures(1, &m_DepthMap);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthMap);

		for (unsigned int i = 0; i < 6; ++i)
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
				m_ShadowMapSize, m_ShadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		
		glGenFramebuffers(1, &m_DepthMapFb);
		glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFb);
		glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthMap, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

EVA::Light::Light(DataObject data) 
	: Light(
	data.Get<std::string>("type", "directional") == "directional" ? Directional : Point,
	data.Get("shadows", true),
	data.Get<int>("shadowMapSize", DEFAULT_SHADOW_MAP_SIZE))
{
	color = data.Get("color", color);
	ambientCoefficient = data.Get("ambientCoefficient", ambientCoefficient);
	
	SetRotation(data.Get("rotation", glm::vec2(0.0f)));

	directionalShadowDistance = data.Get("directionalShadowDistance", directionalShadowDistance);

	directionalNearPlane = data.Get("directionalNearPlane", directionalNearPlane);
	directionalFarPlane = data.Get("directionalFarPlane", directionalFarPlane);

	SetPosition(data.Get("position", glm::vec3(0.0f)));

	attenuation = data.Get("attenuation", attenuation);

	pointNearPlane = data.Get("pointNearPlane", pointNearPlane);
	pointFarPlane = data.Get("pointFarPlane", pointFarPlane);
}

void EVA::Light::SetPosition(const glm::vec3 newPosition)
{
	m_Position = newPosition;
}

void EVA::Light::SetRotation(const glm::vec2 rotation)
{
	m_Rotation = rotation;

	const auto pitch = glm::radians(rotation.x);
	const auto yaw = glm::radians(rotation.y);

    m_Direction.x = std::cos(yaw) * std::cos(pitch);
    m_Direction.y = std::sin(pitch);
    m_Direction.z = std::sin(yaw) * std::cos(pitch);
    m_Direction.w = 0.0f;
}

void EVA::Light::Save(DataObject& data) const
{
	data.Set<std::string>("type", m_Type == Directional ? "directional" : "point");
	data.Set("color", color);
	data.Set("ambientCoefficient", ambientCoefficient);

	if(m_Type == Directional)
	{
		data.Set("rotation", m_Rotation);

		data.Set("directionalShadowDistance", directionalShadowDistance);

		data.Set("directionalNearPlane", directionalNearPlane);
		data.Set("directionalFarPlane", directionalFarPlane);
	}
	else
	{
		data.Set("position", m_Position);

		data.Set("attenuation", attenuation);

		data.Set("pointNearPlane", pointNearPlane);
		data.Set("pointFarPlane", pointFarPlane);
	}

	data.Set("shadows", m_Shadows);
	if(m_Shadows)
	{
		data.Set<int>("shadowMapSize", m_ShadowMapSize);
	}
}

glm::mat4 EVA::Light::GetLightSpaceMatrix() const
{
	const auto lightProjection = glm::ortho(-directionalShadowDistance, directionalShadowDistance, -directionalShadowDistance, directionalShadowDistance, directionalNearPlane, directionalFarPlane);

	const auto lightDirection = -glm::normalize(glm::vec3(GetDirection()));
	const auto cameraPosition = Application::mainCamera->transform->position;


	const auto lightView = glm::lookAt(
		glm::vec3(cameraPosition - lightDirection * (directionalFarPlane / 2)),
		glm::vec3(cameraPosition),
		glm::vec3(0.0f, 1.0f, 0.0f));

	return lightProjection * lightView;
}

std::vector<glm::mat4> EVA::Light::GetShadowTransforms() const
{
	const auto shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, pointNearPlane, pointFarPlane);

	std::vector<glm::mat4> shadowTransforms;
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Position, m_Position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Position, m_Position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Position, m_Position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Position, m_Position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Position, m_Position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
	shadowTransforms.push_back(shadowProj * glm::lookAt(m_Position, m_Position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

	return shadowTransforms;
}
