#include "Light.hpp"

#include "GL/glew.h"
#include "glm/gtc/matrix_transform.hpp"

#include "../Application.hpp"
#include "../Input.hpp"
#include "../Scene.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(Light, "EVA::Light")

	Light::~Light()
	{
		if (scene.Get() != nullptr)
			scene->RemoveLight(this);
	}

	void Light::Awake()
	{
		if (m_Shadows)
			GenerateTexturesaAndFrameBuffer();

		if (scene.Get() != nullptr)
			scene->AddLight(this);
	}

	glm::mat4 Light::GetLightSpaceMatrix() const
	{
		const auto lightProjection = glm::ortho(-directionalShadowDistance, directionalShadowDistance,
		                                        -directionalShadowDistance, directionalShadowDistance, directionalNearPlane,
		                                        directionalFarPlane);

		const auto lightDirection = -normalize(glm::vec3(transform->forward));
		const auto cameraPosition = Application::mainCamera->transform->position;


		const auto lightView = lookAt(
			glm::vec3(cameraPosition - lightDirection * (directionalFarPlane / 2)),
			glm::vec3(cameraPosition),
			glm::vec3(0.0f, 1.0f, 0.0f));

		return lightProjection * lightView;
	}

	std::vector<glm::mat4> Light::GetShadowTransforms() const
	{
		const auto shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, pointNearPlane, pointFarPlane);

		const auto pos = transform->position;

		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.push_back(shadowProj * lookAt(pos, pos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj * lookAt(pos, pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj * lookAt(pos, pos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)));
		shadowTransforms.push_back(shadowProj * lookAt(pos, pos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)));
		shadowTransforms.push_back(shadowProj * lookAt(pos, pos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)));
		shadowTransforms.push_back(shadowProj * lookAt(pos, pos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0)));

		return shadowTransforms;
	}

	void Light::Serialize(DataObject& data)
	{
		std::string type = m_Type == Directional ? "directional" : "point";
		data.Serialize("type", type);
		m_Type = type == "directional" ? Directional : Point;

		data.Serialize("color", color);
		data.Serialize("ambientCoefficient", ambientCoefficient);

		data.Serialize("shadows", m_Shadows);

		int sms = m_ShadowMapSize;
		data.Serialize("shadowMapSize", sms);
		m_ShadowMapSize = sms;

		if (data.mode != DataObject::DataMode::Inspector || m_Type == Directional)
		{
			data.Serialize("directionalShadowDistance", directionalShadowDistance);
			data.Serialize("directionalNearPlane", directionalNearPlane);
			data.Serialize("directionalFarPlane", directionalFarPlane);
		}

		if (data.mode != DataObject::DataMode::Inspector || m_Type == Point)
		{
			data.Serialize("attenuation", attenuation);

			data.Serialize("pointNearPlane", pointNearPlane);
			data.Serialize("pointFarPlane", pointFarPlane);
		}

		if (data.changed)
		{
			if (m_Shadows)
				GenerateTexturesaAndFrameBuffer();
		}
	}

	void Light::GenerateTexturesaAndFrameBuffer()
	{
		if (m_DepthMap != 0)
		{
			GLCall(glDeleteTextures(1, &m_DepthMap));
		}

		if (m_DepthMapFb != 0)
		{
			GLCall(glDeleteFramebuffers(1, &m_DepthMapFb));
		}

		if (m_Type == Directional)
		{
			// Texture
			GLCall(glGenTextures(1, &m_DepthMap));
			GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthMap));
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
				m_ShadowMapSize, m_ShadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));

			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));

			float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
			GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

			// Frame buffer
			GLCall(glGenFramebuffers(1, &m_DepthMapFb));
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFb));
			GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthMap, 0));
			GLCall(glDrawBuffer(GL_NONE));
			GLCall(glReadBuffer(GL_NONE));
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		}
		else
		{
			// Texture
			GLCall(glGenTextures(1, &m_DepthMap));
			GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, m_DepthMap));

			for (unsigned int i = 0; i < 6; ++i)
			{
				GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
					m_ShadowMapSize, m_ShadowMapSize, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
			}

			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
			GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE));

			// Frame buffer
			GLCall(glGenFramebuffers(1, &m_DepthMapFb));
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_DepthMapFb));
			GLCall(glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, m_DepthMap, 0));
			GLCall(glDrawBuffer(GL_NONE));
			GLCall(glReadBuffer(GL_NONE));
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		}
	}
}
