#include "EVA.hpp"
#include "EVA/SceneManager.hpp"
#include "../EVA/Editor/SceneEditor.hpp"

#define EDITOR

/**
 * \brief The entry point of the program
 * \return An exit code indicating how the program exited
 */
int main()
{
#ifdef EDITOR

	#include "../EVA/Editor/SceneEditor.hpp"

	if(!EVA::Application::Init("EVA-Engine"))
		return EXIT_FAILURE;

	EVA::SceneManager::CreateScene<EVA::SceneEditor>();

#else
	
	if(!EVA::Application::Init("EVA-Engine"))
		return EXIT_FAILURE;

	EVA::SceneManager::CreateScene<EVA::Scene>("./assets/scenes/main.scene");

#endif

	EVA::Application::Run();

	return EXIT_SUCCESS;
}
