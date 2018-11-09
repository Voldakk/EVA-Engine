#pragma once

#include "EVA.hpp"
#include "EVA/SceneManager.hpp"
#include "../Editor/SceneEditor.hpp"

namespace EVA 
{

	void EntryPoint(const std::string & scene)
	{

#ifdef EDITOR
		EVA::SceneManager::CreateScene<EVA::SceneEditor>(scene);
#else
		EVA::SceneManager::CreateScene<EVA::Scene>(scene);
#endif

		EVA::Application::Run();
	}

}