#include "Skybox.hpp"

#include "GL/glew.h"

#include "ModelManager.hpp"
#include "Material.hpp"
#include "Components/Transform.hpp"
#include "Application.hpp"
#include "TextureManager.hpp"
#include "ShaderManager.hpp"

#include "Utilities/TextureUtilities.hpp"

namespace EVA
{
	Skybox::Skybox()
	{
		m_Hdr = true;

		m_Model = ModelManager::Primitive(ModelManager::CubeInverted);
		m_Transform = std::make_unique<Transform>();
		m_Material = std::make_unique<SkyBoxMaterial>();
	}

	Skybox::Skybox(DataObject data) : Skybox()
	{
		m_Hdr = data.Get("folderPath", true);

		m_HdrPath = data.Get("hdrPath", std::string(""));

		m_FolderPath = data.Get("folderPath", std::string(""));
		m_FileType = data.Get("fileExtension", std::string(""));

		if (m_Hdr)
			Set(m_HdrPath);
		else
			Set(folderPath, fileType);
	}

	void Skybox::Render() const
	{
		if (m_EnvironmentMap == nullptr)
			return;

		m_Transform->SetPosition(Application::mainCamera->transform->position);
		m_Material->Activate(scene, m_Transform.get());

		GLCall(glDepthFunc(GL_LEQUAL));
		GLCall(glDepthMask(GL_FALSE));
		m_Model->GetMesh(0)->Draw();
		GLCall(glDepthMask(GL_TRUE));
	}

	void Skybox::Set(const FS::path& filePath)
	{
		auto hdrTexture = TextureManager::LoadTexture(filePath, TextureWrapping::ClampToEdge, TextureMinFilter::Linear, TextureMagFilter::Linear);

		if (hdrTexture != nullptr)
		{
			m_HdrPath = filePath;

			m_EnvironmentMap = TextureUtilities::EquirectangularToCubemap(hdrTexture);
			m_IrradianceMap = TextureUtilities::ConvoluteCubemap(m_EnvironmentMap);

			m_Material->cubemap = m_IrradianceMap;
			m_Material->shader = ShaderManager::LoadShader(ShaderManager::STANDARD_SHADERS_PATH / "skybox.shader");
		}
		else
		{
			m_EnvironmentMap = nullptr;
			m_HdrPath = "";
		}
	}

	void Skybox::Set(const std::string& folderPath, const std::string& fileType)
	{
		m_EnvironmentMap = TextureManager::LoadTextureCubemap(folderPath, fileType);
		
		if (m_EnvironmentMap != nullptr)
		{
			m_FolderPath = folderPath;
			m_FileType = fileType;

			m_Material->cubemap = m_EnvironmentMap;
			m_Material->shader = ShaderManager::LoadShader(ShaderManager::STANDARD_SHADERS_PATH / "skybox.shader");
		}
		else
		{
			m_EnvironmentMap = nullptr;
			m_FolderPath = "";
			m_FileType = "";
		}
	}

	void Skybox::SetTint(const glm::vec4 tint) const
	{
		if (m_Material != nullptr)
			m_Material->skyTint = tint;
	}

	void Skybox::SetTint(const glm::vec3 tint) const
	{
		if (m_Material != nullptr)
			m_Material->skyTint = glm::vec4(tint, 1.0f);
	}

	void Skybox::Save(DataObject& data) const
	{
		data.Set("hdr", m_Hdr);

		if (!m_HdrPath.empty())
			data.Set("hdrPath", m_HdrPath);

		if (!m_FolderPath.empty()) 
		{
			data.Set("folderPath", m_FolderPath);
			data.Set("fileExtension", m_FileType);
		}
	}

	void Skybox::DrawInspector()
	{
		InspectorFields::Default("HDR", m_Hdr);

		if (m_Hdr)
		{
			if (InspectorFields::Default("HDR", m_HdrPath))
			{
				Set(m_HdrPath);
			}
		}
		else
		{
			if (InspectorFields::DragDropTargetString("Folder path", m_FolderPath, "folder") ||
				InspectorFields::EnterString("File type", m_FileType))
			{
				Set(m_FolderPath, m_FileType);
			}
		}

		if (m_Material != nullptr)
		{
			InspectorFields::ColorPicker("Tint", m_Material->skyTint);
			InspectorFields::Float("Sun size", m_Material->sunSize);
			InspectorFields::ColorPicker("Sun color", m_Material->sunColor);
		}
	}
}
