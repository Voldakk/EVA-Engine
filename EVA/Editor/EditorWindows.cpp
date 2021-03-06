#include "EditorWindows.hpp"

#include "glm/gtc/type_ptr.hpp"

#include "SceneEditor.hpp"
#include "../Parsers/SceneParser.hpp"
#include "../MaterialManager.hpp"
#include "../Parsers/MaterialParser.hpp"
#include "../ShaderManager.hpp"

namespace EVA
{

	EditorWindows::EditorWindows(SceneEditor* editor)
	{
		m_Editor = editor;

		m_NameBuffer = new char[m_NameBufferSize];
	}

	EditorWindows::~EditorWindows()
	{
		delete[] m_NameBuffer;
	}
	
	void EditorWindows::SceneHierarchy()
	{
		const auto screenSize = Application::GetWindowSize();
		ImGui::SetNextWindowSizeConstraints({ 300.0f, (float)screenSize.y - m_MenuBarHeight }, { (float)screenSize.x, (float)screenSize.y - m_MenuBarHeight });
		ImGui::SetNextWindowPos({ 0.0f, m_MenuBarHeight });

		const auto flags = ImGuiWindowFlags_ResizeFromAnySide /*| ImGuiWindowFlags_NoCollapse*/;
		ImGui::Begin("Scene Hierarchy", nullptr, flags);

		const auto windowSize = ImGui::GetWindowSize();
		m_HierarchyWidth = windowSize.x;

		// Skybox
		if (m_Editor->skybox != nullptr)
		{
			const auto nodeFlags =
				ImGuiTreeNodeFlags_OpenOnDoubleClick |
				ImGuiTreeNodeFlags_OpenOnArrow |
				ImGuiTreeNodeFlags_Leaf |
				(IsSelected(m_Editor->skybox.get()) ? ImGuiTreeNodeFlags_Selected : 0);

			if (ImGui::TreeNodeEx("Skybox", nodeFlags))
			{
				// Select on click
				if (ImGui::IsItemClicked())
					SelectSkybox(m_Editor->skybox.get());

				// Context menu
				if (ImGui::BeginPopupContextItem())
				{
					if (ImGui::MenuItem("Delete"))
					{
						if (IsSelected(m_Editor->skybox.get()))
							SelectSkybox(nullptr);
						m_Editor->skybox = nullptr;
					}

					ImGui::EndPopup();
				}

				ImGui::TreePop();
			}
		}

		// Game objects
		auto gameObjects = m_Editor->GetGameObjects();
		for (auto& gameObject : gameObjects)
		{
			if (gameObject->GetName() == "EVA::EditorCamera")
				continue;

			if (gameObject->transform->parent.Get() == nullptr)
				DisplayGameObjectsRecursively(gameObject.get());
		}

		// Context menu
		if (ImGui::BeginPopupContextWindow(nullptr, 1, false))
		{
			if (ImGui::MenuItem("Add game object"))
			{
				auto newGameObject = m_Editor->CreateGameObject();
				newGameObject->SetName("New GameObject");
			}

			ImGui::EndPopup();
		}

		ImGui::End();
	}

	void EditorWindows::Inspector()
	{
		const auto screenSize = Application::GetWindowSize();
		ImGui::SetNextWindowSizeConstraints({ 300.0f, (float)screenSize.y - m_MenuBarHeight }, { (float)screenSize.x, (float)screenSize.y - m_MenuBarHeight });
		ImGui::SetNextWindowPos({ (float)screenSize.x, m_MenuBarHeight }, 0, { 1.0f, 0.0f });

		const auto flags = ImGuiWindowFlags_ResizeFromAnySide /*| ImGuiWindowFlags_NoCollapse*/;
		ImGui::Begin("Inspector", nullptr, flags);

		const auto windowSize = ImGui::GetWindowSize();
		m_InspectorWidth = windowSize.x;

		switch (m_SelectedType)
		{
		case SelectedType::GameObject:
			GameObjectInspector(windowSize.x);
			break;
		case SelectedType::Light:
			LightInspector();
			break;
		case SelectedType::Skybox:
			SkyboxInspector();
			break;
		case SelectedType::Path:
			{
				const auto extension = FileSystem::ToString(m_SelectedPath.extension());
				if (extension == ".mat")
					MaterialInspector();
				if (extension == ".shader")
					ShaderInspector();
			}
			break;
		default:
			break;
		}

		ImGui::End();
	}

	void EditorWindows::MenuBar()
	{
		bool openNew = false;
		if (ImGui::BeginMainMenuBar())
		{
			m_MenuBarHeight = ImGui::GetWindowSize().y;

			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New"))
				{
					openNew = true;
				}
				if (ImGui::MenuItem("Open"))
				{
					char const * filterPatterns[1] = { "*.scene" };
					const auto path = FileSystem::OpenFileDialog("Open scene", "", 1, filterPatterns);
					if (!path.empty())
					{
						m_Editor->Clear();
						SceneParser::Load(m_Editor, path);
						m_Editor->Awake();
						m_Editor->Start();
					}
				}
				if (ImGui::MenuItem("Save as"))
				{
					char const * filterPatterns[1] = { "*.scene" };
					auto path = FileSystem::SaveFileDialog("Save scene", "", 1, filterPatterns);

					if (!path.empty())
					{
						if (!path.has_extension())
							path += FS::path(".scene");

						SceneParser::Save(m_Editor, path);
					}
				}
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Add"))
			{
				if (ImGui::MenuItem("GameObject"))
				{
					auto newGameObject = m_Editor->CreateGameObject();
					newGameObject->SetName("New GameObject");
				}

				if (ImGui::BeginMenu("Component", SelectedGameObject() != nullptr))
				{
					auto ids = ClassMapEVAComponent::GetIds();

					for (const auto& id : ids)
					{
						if (ImGui::MenuItem(id.c_str()) && SelectedGameObject() != nullptr)
						{
							const auto component = ClassMapEVAComponent::Create(id);
							if (component != nullptr)
							{
								SelectedGameObject()->AttachComponent(component);

								component->Awake();
								component->Start();
							}
						}
					}
					ImGui::EndMenu();
				}

				if (m_Editor->skybox == nullptr)
				{
					if (ImGui::MenuItem("Skybox"))
					{
						m_Editor->skybox = std::make_unique<EVA::Skybox>();
					}
				}


				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}

		if (openNew)
			ImGui::OpenPopup("New scene");

		if (ImGui::BeginPopupModal("New scene", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Clear the current scene?.\nThis operation cannot be undone!\n\n");
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
				m_Editor->LoadTemplate();
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(120, 0)))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}
	}

	void EditorWindows::AssetBrowser()
	{
		const auto screenSize = Application::GetWindowSize();

		ImGui::SetNextWindowSizeConstraints(
			{ (float)screenSize.x - m_HierarchyWidth - m_InspectorWidth, 200.0f },
			{ (float)screenSize.x - m_HierarchyWidth - m_InspectorWidth, (float)screenSize.y - m_MenuBarHeight });

		ImGui::SetNextWindowPos({ m_HierarchyWidth, (float)screenSize.y }, 0, { 0.0f, 1.0f });

		const auto flags = ImGuiWindowFlags_ResizeFromAnySide/* | ImGuiWindowFlags_NoCollapse*/ | ImGuiWindowFlags_MenuBar;
		ImGui::Begin("Assets", nullptr, flags);

		// Folder tree
		ImGui::BeginChild(ImGui::GetID((void*)nullptr), ImVec2(200.0f, ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y), true);

		DisplayFoldersRecursively("./assets");

		ImGui::EndChild();
		ImGui::SameLine();

		// Current folder view
		ImGui::BeginChild(ImGui::GetID((void*)1), ImVec2(ImGui::GetWindowWidth() - 225.0f, ImGui::GetWindowContentRegionMax().y - ImGui::GetWindowContentRegionMin().y), true);

		// Window context menu
		if (ImGui::BeginPopupContextWindow("Asset folder context"))
		{
			if (ImGui::BeginMenu("New"))
			{
				if (ImGui::MenuItem("Folder"))
				{
					m_NewAssetType = NewAssetType::Folder;
					m_NewPath = m_SelectedAssetFolder;
					strcpy_s(m_NameBuffer, m_NameBufferSize, "New folder");
				}
				if (ImGui::MenuItem("Material"))
				{
					m_NewAssetType = NewAssetType::Material;
					m_NewPath = m_SelectedAssetFolder;
					strcpy_s(m_NameBuffer, m_NameBufferSize, "New material");
				}
				if (ImGui::MenuItem("Shader"))
				{
					m_NewAssetType = NewAssetType::Shader;
					m_NewPath = m_SelectedAssetFolder;
					strcpy_s(m_NameBuffer, m_NameBufferSize, "New Shader");
				}
				ImGui::EndMenu();
			}
			ImGui::EndPopup();
		}

		// Back button
		if(m_SelectedAssetFolder != "./assets")
		{
			if (ImGui::Selectable(".."))
			{
				m_SelectedAssetFolder = m_SelectedAssetFolder.parent_path();
			}
		}

		// Files and folders list
		for (const auto& p : FS::directory_iterator(m_SelectedAssetFolder))
		{
			ImGui::BeginGroup();
			if (is_directory(p))
			{
				if (ImGui::Selectable(("-> " + p.path().stem().string()).c_str()))
				{
					m_SelectedAssetFolder = p.path();
				}
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
				{
					ImGui::Text(p.path().stem().string().c_str());
					const auto pathString = FileSystem::ToString(p.path());
					ImGui::SetDragDropPayload("folder", pathString.c_str(), sizeof(char) * pathString.length() + 1, ImGuiCond_Once);
					ImGui::EndDragDropSource();
				}
			}
			else
			{
				if (ImGui::Selectable(p.path().filename().string().c_str()))
				{
					SelectPath(p.path());
				}
				if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
				{
					ImGui::Text(p.path().filename().string().c_str());
					const auto pathString = FileSystem::ToString(p.path());
					ImGui::SetDragDropPayload("file", pathString.c_str(), sizeof(char) * pathString.length() + 1, ImGuiCond_Once);
					ImGui::EndDragDropSource();
				}
			}

			// Item context menu
			if (ImGui::BeginPopupContextItem(FileSystem::ToString(p.path()).c_str()))
			{
				if (ImGui::MenuItem("Rename"))
				{
					m_RenamePath = p.path();
					strcpy_s(m_NameBuffer, m_NameBufferSize, m_RenamePath.filename().string().c_str());
					
				}
				if (ImGui::MenuItem("Delete"))
				{
					m_DeletePath = p.path();
				}
				ImGui::EndPopup();
			}
			ImGui::EndGroup();
		}

		ImGui::EndChild();

		ImGui::End();

		// New asset popup
		if (!m_NewPath.empty())
			ImGui::OpenPopup("New asset");

		if (ImGui::BeginPopupModal("New asset", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			NewAssetPopup();
			ImGui::EndPopup();
		}

		// Rename popup
		if(!m_RenamePath.empty())
			ImGui::OpenPopup("Rename");

		if (ImGui::BeginPopupModal("Rename", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			RenameAssetPopup();
			ImGui::EndPopup();
		}

		// Delete popup
		if (!m_DeletePath.empty())
			ImGui::OpenPopup("Delete");

		if (ImGui::BeginPopupModal("Delete", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			DeleteAssetPopup();
			ImGui::EndPopup();
		}
	}

	void EditorWindows::SelectGameObject(EVA::GameObject* gameObject)
	{
		m_SelectedType = SelectedType::GameObject;
		m_SelectedGameObject = gameObject;
	}

	bool EditorWindows::IsSelected(EVA::GameObject* gameObject) const
	{
		return m_SelectedType == SelectedType::GameObject && m_SelectedGameObject == gameObject;
	}

	EVA::GameObject* EditorWindows::SelectedGameObject() const
	{
		return m_SelectedType == SelectedType::GameObject ? m_SelectedGameObject : nullptr;
	}

	void EditorWindows::SelectLight(EVA::Light* light)
	{
		m_SelectedType = SelectedType::Light;
		m_SelectedLight = light;
	}

	bool EditorWindows::IsSelected(EVA::Light* light) const
	{
		return m_SelectedType == SelectedType::Light && m_SelectedLight == light;
	}

	EVA::Light* EditorWindows::SelectedLight() const
	{
		return m_SelectedType == SelectedType::Light ? m_SelectedLight : nullptr;
	}

	void EditorWindows::SelectSkybox(EVA::Skybox* skybox)
	{
		m_SelectedType = SelectedType::Skybox;
		m_SelectedSkybox = skybox;
	}

	bool EditorWindows::IsSelected(EVA::Skybox* skybox) const
	{
		return m_SelectedType == SelectedType::Skybox && m_SelectedSkybox == skybox;
	}

	EVA::Skybox* EditorWindows::SelectedSkybox() const
	{
		return m_SelectedType == SelectedType::Skybox ? m_SelectedSkybox : nullptr;
	}

	void EditorWindows::SelectPath(const FS::path& path)
	{
		m_SelectedType = SelectedType::Path;
		m_SelectedPath = path;
	}

	bool EditorWindows::IsSelected(const FS::path& path) const
	{
		return m_SelectedType == SelectedType::Path && m_SelectedPath == path;
	}

	FS::path EditorWindows::SelectedPath() const
	{
		return m_SelectedType == SelectedType::Path ? m_SelectedPath : "";
	}

	void EditorWindows::DisplayFoldersRecursively(const FS::path& path)
	{
		auto hasChildDirectories = false;
		for (auto& p : FS::directory_iterator(path))
		{
			if (is_directory(p))
			{
				hasChildDirectories = true;
				break;
			}
		}

		const auto nodeFlags =
			ImGuiTreeNodeFlags_OpenOnDoubleClick |
			ImGuiTreeNodeFlags_OpenOnArrow |
			(path == m_SelectedAssetFolder ? ImGuiTreeNodeFlags_Selected : 0) |
			(hasChildDirectories ? 0 : ImGuiTreeNodeFlags_Leaf);

		if (ImGui::TreeNodeEx(path.stem().string().c_str(), nodeFlags))
		{
			// Select on click
			if (ImGui::IsItemClicked())
				m_SelectedAssetFolder = path;

			for (auto& p : FS::directory_iterator(path))
			{
				if (is_directory(p))
				{
					DisplayFoldersRecursively(p.path());
				}
			}

			ImGui::TreePop();
		}
	}

	void EditorWindows::DisplayGameObjectsRecursively(EVA::GameObject* gameObject)
	{
		const auto nodeFlags =
			ImGuiTreeNodeFlags_OpenOnDoubleClick |
			ImGuiTreeNodeFlags_OpenOnArrow |
			(IsSelected(gameObject) ? ImGuiTreeNodeFlags_Selected : 0) |
			(gameObject->transform->GetChildren().empty() ? ImGuiTreeNodeFlags_Leaf : 0);

		if (ImGui::TreeNodeEx(gameObject->GetName().c_str(), nodeFlags))
		{
			// Select on click
			if (ImGui::IsItemClicked())
				SelectGameObject(gameObject);

			// Context menu
			if (ImGui::BeginPopupContextItem())
			{
				if (ImGui::MenuItem("Add game object"))
				{
					auto newGameObject = m_Editor->CreateGameObject();
					newGameObject->SetName("New GameObject");
					newGameObject->SetParent(gameObject);
				}
				if (ImGui::MenuItem("Delete"))
				{
					gameObject->Destroy();
					if (IsSelected(gameObject))
						SelectGameObject(nullptr);

					ImGui::EndPopup();
					ImGui::TreePop();
					return;
				}

				ImGui::EndPopup();
			}

			// Display children
			for (auto& child : gameObject->transform->GetChildren())
			{
				DisplayGameObjectsRecursively(child->gameObject.Get());
			}

			ImGui::TreePop();
		}
	}

	void EditorWindows::GameObjectInspector(const float width) const
	{
		auto gameObject = SelectedGameObject();

		if (gameObject == nullptr)
			return;

		ImGui::PushItemWidth(width);

		// Name
		ImGui::Text("Name");
		const auto gameObjectName = InspectorFields::GetCString(gameObject->GetName());
		if (ImGui::InputText("##Line", gameObjectName, 10000, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			gameObject->SetName(gameObjectName);
		}

		ImGui::Spacing();

		// Transform
		ImGui::Text("Position");
		auto position = gameObject->transform->localPosition;
		ImGui::InputFloat3("InspectorTransformPosition", glm::value_ptr(position), "%.7g");

		gameObject->transform->SetPosition(position);

		ImGui::Text("Rotation");
		auto rotation = gameObject->transform->localRotation;
		if(ImGui::InputFloat3("InspectorTransformRotation", glm::value_ptr(rotation), "%.7g"))
			gameObject->transform->SetOrientation(rotation);


		ImGui::Text("Scale");
		auto scale = gameObject->transform->localScale;
		ImGui::InputFloat3("InspectorTransformScale", glm::value_ptr(scale), "%.7g");
		gameObject->transform->SetScale(scale);

		ImGui::PopItemWidth();

		// Other components
		ImGui::Text("Components");
		const auto components = gameObject->GetComponents();
		for (unsigned int i = 0; i < components.size(); ++i)
		{
			auto component = components[i];
			bool keep = true;
			if (ImGui::CollapsingHeader((std::to_string(i) + " " + component->GetTypeId()).c_str(), &keep))
			{
				ImGui::PushID(i);

				DataObject d;
				d.mode = DataObject::DataMode::Inspector;
				component->Serialize(d);

				ImGui::PopID();
			}
			if (!keep)
			{
				gameObject->RemoveComponent(component.get());
			}
		}

		ImGui::PushItemWidth(width);

		// Add component
		if (ImGui::Button("Add Component"))
		{
			ImGui::OpenPopup("InspectorAddComponentSelect");
		}

		if (ImGui::BeginPopup("InspectorAddComponentSelect"))
		{
			auto ids = ClassMapEVAComponent::GetIds();
			ImGui::Separator();

			for (const auto& id : ids)
			{
				if (ImGui::Selectable(id.c_str()))
				{
					const auto component = ClassMapEVAComponent::Create(id);
					if (component != nullptr)
					{
						gameObject->AttachComponent(component);

						component->Awake();
						component->Start();
					}
				}
			}
			ImGui::EndPopup();
		}

		ImGui::PopItemWidth();
	}

	void EditorWindows::LightInspector() const
	{
		auto light = SelectedLight();
		if (light == nullptr)
			return;

		DataObject d;
		d.mode = DataObject::DataMode::Inspector;
		light->Serialize(d);
	}

	void EditorWindows::SkyboxInspector() const
	{
		auto skybox = SelectedSkybox();

		if (skybox == nullptr)
			return;

		ImGui::Text("Skybox");

		skybox->DrawInspector();
	}

	void EditorWindows::MaterialInspector() const
	{
		auto material = MaterialManager::LoadMaterial(m_SelectedPath);

		if (material == nullptr)
			return;

		DataObject d;
		d.mode = DataObject::DataMode::Inspector;

		material->Serialize(d);

		if (d.changed)
			material->SaveToFile();
	}

	void EditorWindows::ShaderInspector() const
	{
		auto shader = ShaderManager::LoadShader(m_SelectedPath);

		if (shader == nullptr)
			return;

		ImGui::Text(("Shader: " + FileSystem::ToString(shader->paths->shader.filename())).c_str());

		auto paths = shader->paths;
		
		auto path = FileSystem::ToString(shader->paths->vertex);
		if (InspectorFields::DragDropTargetString("Vertex", path, "file"))
		{
			paths->vertex = path;
			shader->SetPaths(paths);
			ShaderManager::SaveShader(shader.get(), paths->shader);
		}

		path = FileSystem::ToString(shader->paths->fragment);
		if (InspectorFields::DragDropTargetString("Fragment", path, "file"))
		{
			paths->fragment = path;
			shader->SetPaths(paths);
			ShaderManager::SaveShader(shader.get(), paths->shader);
		}

		path = FileSystem::ToString(shader->paths->geometry);
		if (InspectorFields::DragDropTargetString("Geometry", path, "file"))
		{
			paths->geometry = path;
			shader->SetPaths(paths);
			ShaderManager::SaveShader(shader.get(), paths->shader);
		}

		path = FileSystem::ToString(shader->paths->tessControl);
		if (InspectorFields::DragDropTargetString("Tessellation control", path, "file"))
		{
			paths->tessControl = path;
			shader->SetPaths(paths);
			ShaderManager::SaveShader(shader.get(), paths->shader);
		}

		path = FileSystem::ToString(shader->paths->tessEvaluation);
		if (InspectorFields::DragDropTargetString("Tessellation evaluation", path, "file"))
		{
			paths->tessEvaluation = path;
			shader->SetPaths(paths);
			ShaderManager::SaveShader(shader.get(), paths->shader);
		}

		if (InspectorFields::Button("Reload"))
		{
			shader->SetPaths(paths);
		}
	}

	void EditorWindows::NewAssetPopup()
	{
		if (ImGui::InputText("##NewName", m_NameBuffer, m_NameBufferSize, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			auto path = m_NewPath / m_NameBuffer;
			switch (m_NewAssetType)
			{
			case NewAssetType::Folder:
			{
				FS::create_directory(path);
				break;
			}
			case NewAssetType::Material:
			{
				const auto newMaterial = std::make_shared<EVA::Material>();

				path.replace_extension(".mat");
				MaterialParser::Save(newMaterial, path);
			}
			break;
			case NewAssetType::Shader:
			{
				const auto newShader = std::make_shared<EVA::Shader>();

				path.replace_extension(".shader");
				ShaderManager::SaveShader(newShader.get(), path);
			}
			break;
			default:;
			}

			m_NewPath = "";
			ImGui::CloseCurrentPopup();
		}

		ImGui::Separator();

		if (ImGui::Button("Ok", ImVec2(120, 0)))
		{
			auto path = m_NewPath / m_NameBuffer;
			switch (m_NewAssetType)
			{
			case NewAssetType::Folder:
			{
				FS::create_directory(path);
				break;
			}
			case NewAssetType::Material:
			{
				const auto newMaterial = std::make_shared<EVA::Material>();

				path.replace_extension(".mat");
				MaterialParser::Save(newMaterial, path);
			}
			break;
			case NewAssetType::Shader:
			{
				const auto newShader = std::make_shared<EVA::Shader>();

				path.replace_extension(".shader");
				ShaderManager::SaveShader(newShader.get(), path);
			}
			break;
			default:;
			}

			m_NewPath = "";
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel"))
		{
			m_NewPath = "";
			ImGui::CloseCurrentPopup();
		}
	}

	void EditorWindows::RenameAssetPopup()
	{
		ImGui::Text(m_RenamePath.filename().string().c_str());

		if (ImGui::InputText("##NewName", m_NameBuffer, 10000, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (m_SelectedPath == m_RenamePath)
				SelectPath("");

			const auto newPath = m_RenamePath.parent_path() / m_NameBuffer;

			try
			{
				FS::rename(m_RenamePath, newPath);
			}
			catch (const std::exception& e)
			{
				std::cout << "EditorWindows::RenameAssetPopup - Failed to rename asset: '" << m_RenamePath << "' to '" << newPath << "'" << std::endl
					<< e.what() << std::endl;
			}

			m_RenamePath = "";
			ImGui::CloseCurrentPopup();
		}

		ImGui::Separator();

		if (ImGui::Button("Ok", ImVec2(120, 0)))
		{
			if (m_SelectedPath == m_RenamePath)
				SelectPath("");

			const auto newPath = m_RenamePath.parent_path() / m_NameBuffer;

			try
			{
				FS::rename(m_RenamePath, newPath);
			}
			catch (const std::exception& e)
			{
				std::cout << "EditorWindows::RenameAssetPopup - Failed to rename asset: '" << m_RenamePath << "' to '" << newPath << "'" << std::endl
					<< e.what() << std::endl;
			}

			m_RenamePath = "";
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			m_RenamePath = "";
			ImGui::CloseCurrentPopup();
		}
	}

	void EditorWindows::DeleteAssetPopup()
	{
		ImGui::Text(m_DeletePath.filename().string().c_str());

		ImGui::Separator();

		if (ImGui::Button("Ok", ImVec2(120, 0)))
		{
			if (m_SelectedPath == m_DeletePath)
				SelectPath("");

			try
			{
				FS::remove(m_DeletePath);
			}
			catch(const std::exception& e)
			{
				std::cout << "EditorWindows::DeleteAssetPopup - Failed to delete asset: " << m_DeletePath << std::endl
				<< e.what() << std::endl;
			}

			m_DeletePath = "";
			ImGui::CloseCurrentPopup();
		}
		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			m_DeletePath = "";
			ImGui::CloseCurrentPopup();
		}
	}
}
