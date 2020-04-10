#pragma once

#include <string>
#include <memory>
#include "Parsers/Json.hpp"

#include "Materials/SkyBoxMaterial.hpp"

namespace EVA
{
	class Model;
	class Shader;
	class Transform;

	/**
	 * \brief A skybox
	 */
	class Skybox
	{
		std::shared_ptr<Texture> m_EnvironmentMap;
		std::shared_ptr<Texture> m_IrradianceMap;

		std::shared_ptr<Model> m_Model;
		std::unique_ptr<SkyBoxMaterial> m_Material;
		std::unique_ptr<Transform> m_Transform;

		bool m_Hdr;

		// HDR
		FS::path m_HdrPath;

		// Not HDR
		std::string m_FolderPath;
		std::string m_FileType;

	public:

		const std::string& folderPath = m_FolderPath;
		const std::string& fileType = m_FileType;

		Scene* scene;

		Skybox();
		explicit Skybox(DataObject data);

		void Render() const;

		void Set(const FS::path& filePath);

		void Set(const std::string& folderPath, const std::string& fileType);

		void SetTint(glm::vec4 tint) const;

		void SetTint(glm::vec3 tint) const;

		/**
		* \brief Saves Skybox values to the given DataObject
		* \param data The DataObject
		*/
		void Save(DataObject& data) const;

		void DrawInspector();
	};

}
