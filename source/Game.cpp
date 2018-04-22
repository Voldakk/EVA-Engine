#include "Game.hpp"

#include <iostream>

#include "EVA/Graphics.hpp"
#include "EVA/SceneManager.hpp"
#include "EVA/ResourceManagers.hpp"
#include "EVA/UI.hpp"

#include "TileMap.hpp"
#include "GhostVariations.hpp"
#include "Pacman.hpp"

Game::Game()
{
	// Shaders
	EVA::ShaderManager::CreateOrGetShader("standard", "standard.vert", "standard.frag");
	EVA::ShaderManager::CreateOrGetShader("standard_instanced", "standard_instanced.vert", "standard_instanced.frag");

	EVA::ShaderManager::CreateOrGetShader("standard_transparent", "standard.vert", "standard.frag");

	// Skybox
	skybox = std::make_unique<EVA::Skybox>("./assets/ame_ash/", ".tga");

	// A directional light that fits the skybox
	auto directionalLight = CreateLight(EVA::LightType::Directional, true);
	directionalLight->SetRotation({ 60.0f, -80.0 });
	directionalLight->directionalShadowDistance = 25.0f;

	// Waves
	m_Waves.push_back({ GhostState::Scatter, 7.0f });
	m_Waves.push_back({ GhostState::Chase,  20.0f });
	m_Waves.push_back({ GhostState::Scatter, 7.0f });
	m_Waves.push_back({ GhostState::Chase,  20.0f });
	m_Waves.push_back({ GhostState::Scatter, 5.0f });
	m_Waves.push_back({ GhostState::Chase,  20.0f });
	m_Waves.push_back({ GhostState::Scatter, 5.0f });
	m_Waves.push_back({ GhostState::Chase,   0.0f });

	for (auto& ghost : ghosts)
	{
		ghost->SetState(CurrentWave().state);
	}
	std::cout << "New wave: " << (CurrentWave().state == GhostState::Scatter ? "Scatter" : "Chase") << ", time: " << CurrentWave().time << "\n";

	// Tilemap
	tileMap = CreateGameObject()->AddComponent<TileMap>(this);
	tileMap->ReadFile("./assets/levels/level1.txt");

	// Camera
	auto camera = CreateGameObject()->AddComponent<EVA::Camera>();
	camera->transform->SetPosition({ 0.0f, 3.0f, -2.0f });
	EVA::Application::mainCamera = camera;

	// Pacman
	pacman = CreateGameObject()->AddComponent<Pacman>(tileMap);

	// Shadow
	const auto shadow = CreateGameObject()->AddComponent<GhostShadow>(this);
	ghosts.push_back(shadow);

	//Speedy
	const auto speedy = CreateGameObject()->AddComponent<GhostSpeedy>(this);
	ghosts.push_back(speedy);

	// Bashful
	const auto bashful = CreateGameObject()->AddComponent<GhostBashful>(this, shadow);
	ghosts.push_back(bashful);

	// Pokey
	const auto pokey = CreateGameObject()->AddComponent<GhostPokey>(this);
	ghosts.push_back(pokey);

	// UI
	//m_ScoreLabel = CreateUiElement<EVA::Label>("Score: " + std::to_string(m_Score));
}

void Game::Update(const float deltaTime)
{
	Scene::Update(deltaTime);

	if (EVA::Input::KeyDown(GLFW_KEY_ENTER))
		EVA::SceneManager::ChangeScene<Game>();

	// Waves
	m_WaveTimer += deltaTime;
	if(m_WaveTimer >= CurrentWave().time && m_CurrentWave < m_Waves.size() - 1)
	{
		m_CurrentWave++;
		m_WaveTimer = 0.0f;
		for (auto& ghost : ghosts)
		{
			if (ghost->state == GhostState::Dead)
				continue;

			ghost->SetState(CurrentWave().state);
		}

		std::cout << "New wave: " << (CurrentWave().state == GhostState::Scatter ? "Scatter" : "Chase") << ", time: " << CurrentWave().time << "\n";
	}

	// Energizer
	if(m_ActiveEnergizer)
	{
		m_EnergizerTimer -= deltaTime;
		if(m_EnergizerTimer <= 0)
		{
			m_ActiveEnergizer = false;
			m_Time = 0.0f;

			for (auto& ghost : ghosts)
			{
				if (ghost->state == GhostState::Dead)
					continue;

				ghost->ResetState();
			}
		}
		else
		{
			m_Time += deltaTime;

			for (auto& ghost : ghosts)
			{
				if (ghost->state != GhostState::Frightened)
					continue;

				if(m_EnergizerTimer <= 2.0f)
					ghost->SetMaterialColor(glm::vec3(glm::vec2(glm::sin(m_Time * 10.0f) + 0.5f), 1.0f));
				else
					ghost->SetMaterialColor(glm::vec3(0.0f, 0.0f, 1.0f));
			}
		}
	}

	// Pacman vs ghosts
	for (auto& ghost : ghosts)
	{
		if (ghost->state == GhostState::Dead)
			continue;

		if(ghost->currentTile == pacman->currentTile)
		{
			if(ghost->state == GhostState::Frightened)
			{
				ghost->SetState(GhostState::Dead);
				AddScore(1000);
			}
			else
			{
				// Die
			}
		}
	}
}

Game::Wave Game::CurrentWave()
{
	if(m_CurrentWave < m_Waves.size())
		return m_Waves[m_CurrentWave];

	return {};
}

void Game::AddScore(const unsigned int amount)
{
	m_Score += amount;
	m_ScoreLabel->SetText("Score: " + std::to_string(m_Score));
}

void Game::ActivateEnergizer(const float time)
{
	m_EnergizerTimer = time;
	m_ActiveEnergizer = true;

	for (auto& ghost : ghosts)
	{
		if (ghost->state == GhostState::Dead)
			continue;

		ghost->SetState(GhostState::Frightened);
	}
}
