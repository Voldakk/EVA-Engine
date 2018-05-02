#pragma once

#include "glm/glm.hpp"

#include "../Component.hpp"
#include "../ComponentMap.hpp"

namespace EVA
{

    class GameObject;

    class Camera : public Component, public ILateUpdateComponent
    {
		//const ComponentRegister<Camera> m_Register = ComponentRegister<Camera>("EVA::Camera");

		glm::mat4 m_ViewMatrix;

    public:

        float fov;
        float near;
        float far;

		const glm::mat4& viewMatrix = m_ViewMatrix;

        Camera() = default;
		~Camera();

		void Start() override;

        void LateUpdate() override;
    };

}
