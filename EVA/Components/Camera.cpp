#include "Camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

#include "../GameObject.hpp"
#include "../Application.hpp"

namespace EVA
{
	Camera::~Camera()
	{
		if (Application::mainCamera == this)
			Application::UseDefaultCamera();
	}

	void Camera::Start()
	{
		EVA::Application::SetMainCamera(this);
	}

	void Camera::LateUpdate()
    {
    	// Update view
		m_ViewMatrix = glm::lookAt(transform->position, transform->position + transform->forward, transform->up);
		EVA::Application::SetMainCamera(this);
    }

	void Camera::Serialize(DataObject& data)
	{
		data.Serialize("fov", fov);
		data.Serialize("near", near);
		data.Serialize("far", far);
	}
}
