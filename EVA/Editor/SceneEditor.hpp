#pragma once

#include "../Scene.hpp"

#include <iostream>
#include <memory>

#include "EditorWindows.hpp"
#include "../Parsers/SceneParser.hpp"
#include "../Components/SceneCamera.hpp"
#include "../Physics.hpp"
#include "../Terrain/Terrain.hpp"
#include "../Water/Water.hpp"

namespace EVA
{

	class SceneEditor : public Scene
	{
		GameObject* m_SceneCameraGameObject;
		SceneCamera* m_SceneCamera;

		std::unique_ptr<EditorWindows> m_Ew;

		const char* m_TemplateScenePath = "./assets/standard_assets/scenes/template.scene";

	public:

		void Clear()
		{
			m_GameObjects.clear();
			m_NameMap.clear();
			m_NameCounter = 0;
			m_Lights.clear();
			skybox = nullptr;
			m_Ew->SelectGameObject(nullptr);
			m_Started = false;

			// Camera
			m_SceneCameraGameObject = CreateGameObject().get();
			m_SceneCameraGameObject->SetName("EVA::EditorCamera");
			m_SceneCameraGameObject->AddComponent<Camera>();
			m_SceneCamera = m_SceneCameraGameObject->AddComponent<SceneCamera>();

			m_SceneCameraGameObject->transform->SetPosition({20.0f, 70.0f, 50.0f});
			m_SceneCamera->yaw = 90.0f;
			m_SceneCamera->pitch = -80.0f;
			m_SceneCamera->FreeLook::Update(0.0f);
		}

		void LoadTemplate()
		{
			Clear();

			SceneParser::Load(this, m_TemplateScenePath);

			Awake();
			Start();
		}

		explicit SceneEditor(const FS::path& path = "")
		{
			// Windows
			m_Ew = std::make_unique<EditorWindows>(this);

			Clear();

			if (path.empty())
				LoadTemplate();
			else
				SceneParser::Load(this, path);
		}

		void Update(const float deltaTime) override
		{
			// Move camera
			m_SceneCameraGameObject->Update(deltaTime);

			// Check for clicked object
			if (Input::GetMouseButtonDown(Input::Mouse::MouseLeft) && !ImGui::IsMouseHoveringAnyWindow())
			{
				const auto mousePos = Input::MousePosition();
				const auto ray = Physics::ScreenPosToWorldRay(mousePos, Application::mainCamera);

				RaycastHit hit;
				if (Physics::Raycast(ray, hit, this))
				{
					std::cout << "Name: " << hit.hitCollider->gameObject->GetName() << ", Dist: " << hit.distance
						<< ", Point: (" << hit.point.x << ", " << hit.point.y << ", " << hit.point.z << ") \n";

					m_Ew->SelectGameObject(hit.hitCollider->gameObject.Get());
				}
			}

			// Windows
			m_Ew->SceneHierarchy();
			m_Ew->Inspector();
			m_Ew->MenuBar();
			m_Ew->AssetBrowser();

			// Usually done in Scene::Update
			ProcessDestroyQueue();


			auto t = FindComponentOfType<Terrain>();
			if (t)
				t->Update(deltaTime);

			auto w = FindComponentOfType<Water>();
			if (w)
				w->Update(deltaTime);
		}

		void LateUpdate() override
		{
			// Move camera
			if(m_SceneCameraGameObject != nullptr)
				m_SceneCameraGameObject->LateUpdate();
		}
	};

}
