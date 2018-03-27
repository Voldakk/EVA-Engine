#include "SceneManager.hpp"

namespace EVA
{

	std::vector<std::shared_ptr<Scene>> SceneManager::m_Scenes;

	void SceneManager::LoadScene(const std::shared_ptr<Scene> &scene)
	{
		if (GetIndex(scene) == -1)
		{
			Input::Clear();

			std::cout << "SceneManager::LoadScene - Loading scene: " << typeid(*scene).name() << " \n";
			scene->self = scene;
			m_Scenes.push_back(scene);
		}
	}

	void SceneManager::UnloadScene(const std::shared_ptr<Scene> &scene)
	{
		const auto i = GetIndex(scene);
		if (i != -1)
		{
			std::cout << "SceneManager::UnloadScene - Unloading scene: " << typeid(*scene).name() << " \n";
			m_Scenes.erase(m_Scenes.begin() + i);
		}
	}

	void SceneManager::ChangeScene(const std::shared_ptr<Scene> &scene)
	{
		std::cout << "SceneManager::ChangeScene - Changing scene: " << typeid(*scene).name() << " \n";
		ClearScenes();

		Input::Clear();

		LoadScene(scene);
	}

	void SceneManager::Update(const float deltaTime)
	{
		for (auto &scene : m_Scenes)
		{
			scene->Update(deltaTime);
		}
	}

	void SceneManager::Render()
	{
		for (auto &scene : m_Scenes)
		{
			scene->Render();
		}
	}

	int SceneManager::GetIndex(const std::shared_ptr<Scene> &scene)
	{
		for (size_t i = 0; i < m_Scenes.size(); i++)
		{
			if (scene == m_Scenes[i])
				return i;
		}
		return -1;
	}

	void SceneManager::ClearScenes()
	{
		for (auto &scene : m_Scenes)
		{
			scene->self.reset();
		}

		m_Scenes.clear();
	}

}