#include "Camera.hpp"

#include "glm/glm/gtc/matrix_transform.hpp"

#include "Input.hpp"
#include "GameObject.hpp"

namespace EVA
{

    Camera::Camera(GameObject *gameObject) : Component(gameObject)
    {
        m_Fov = 90.0f;
        m_Near = 0.1f;
        m_Far = 1000.0f;

        m_MouseSensitivity = 0.5f;
        m_MovementSpeed = 5.0f;

        UpdateDirections();
    }

    void Camera::Update(const float deltaTime)
    {
        // Movement
        auto position = m_GameObject->GetTransform()->localPosition;

        if (Input::Key(GLFW_KEY_W)) // Front
            position += m_Front * m_MovementSpeed * deltaTime;
        if (Input::Key(GLFW_KEY_S))    // Back
            position -= m_Front * m_MovementSpeed * deltaTime;

        if (Input::Key(GLFW_KEY_D))    // Right
            position += m_Right * m_MovementSpeed * deltaTime;
        if (Input::Key(GLFW_KEY_A))    // Left
            position -= m_Right * m_MovementSpeed * deltaTime;

        if (Input::Key(GLFW_KEY_SPACE)) // Up
            position += m_Up * m_MovementSpeed * deltaTime;
        if (Input::Key(GLFW_KEY_LEFT_SHIFT)) // Down
            position -= m_Up * m_MovementSpeed * deltaTime;

        m_GameObject->GetTransform()->SetPosition(position);

        // Look
        auto rotation = m_GameObject->GetTransform()->localRotation;

        const auto mouseMovement = Input::MouseMovement();
        rotation += glm::vec3(-mouseMovement.y, mouseMovement.x, 0) * m_MouseSensitivity * deltaTime;

        if (rotation.x < glm::radians(-89.0f))
            rotation.x = glm::radians(-89.0f);
        else if (rotation.x > glm::radians(89.0f))
            rotation.x = glm::radians(89.0f);

        m_GameObject->GetTransform()->SetRotation(rotation);

        // Update view
        UpdateDirections();
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return m_ViewMatrix;
    }

    void Camera::UpdateDirections()
    {
        const auto position = m_GameObject->GetTransform()->position;

        const auto pitch = m_GameObject->GetTransform()->rotation.x;
        const auto yaw = m_GameObject->GetTransform()->rotation.y;

        m_Front.x = static_cast<float>(cos(yaw) * cos(pitch));
        m_Front.y = static_cast<float>(sin(pitch));
        m_Front.z = static_cast<float>(sin(yaw) * cos(pitch));
        m_Front = glm::normalize(m_Front);

        m_Right = glm::normalize(glm::cross(m_Front, {0.0f, 1.0f, 0.0f}));
        m_Up = glm::normalize(glm::cross(m_Right, m_Front));

        m_ViewMatrix = glm::lookAt(position, position + m_Front, m_Up);
    }

}