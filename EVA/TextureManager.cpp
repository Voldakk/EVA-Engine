#include "TextureManager.hpp"

#include <iostream>
#include "GL/glew.h"
#include "stb_image.h"

namespace EVA
{
	std::map<FS::path, std::shared_ptr<Texture>> TextureManager::m_Textures;

	std::shared_ptr<Texture> TextureManager::LoadTexture(const FS::path& path)
	{
		// Return the id if the texture's already loaded
		if (m_Textures.count(path))
			return m_Textures[path];

		auto texture = std::make_shared<Texture>();
		texture->path = path;

		// Load the image
		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		const auto data = stbi_load(FileSystem::ToString(path).c_str(), &width, &height, &channels, 0);

		// If the image was loaded
		if (data)
		{
			// Create texture
			glGenTextures(1, &texture->id);
			glBindTexture(GL_TEXTURE_2D, texture->id);

			// Texture parameters
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

			// Save the texture
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);

			// Save the id
			m_Textures[path] = texture;
			
			std::cout << "TextureManager::GetTexture - Loaded image: " << path.c_str() << "\n";
			std::cout << "TextureManager::GetTexture - Texture id:   " << texture->id << "\n";

			stbi_image_free(data);
			return texture;
		}
		else // If not
		{
			std::cout << "TextureManager::GetTexture - Failed to load image: " << path.c_str() << "\n";
			return nullptr;
		}
	}

	std::shared_ptr<Texture> TextureManager::LoadTextureCubemap(const FS::path& folderPath, const std::string &fileType)
	{
		if (folderPath.empty())
			return nullptr;

		const auto path = folderPath / fileType;

		// Return the id if the texture's already loaded
		if (m_Textures.count(path))
			return m_Textures[path];

		auto texture = std::make_shared<Texture>();
		texture->path = path;

		// Create texture
		glActiveTexture(GL_TEXTURE0);
		glEnable(GL_TEXTURE_CUBE_MAP);
		glGenTextures(1, &texture->id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		// Load files
		stbi_set_flip_vertically_on_load(false);

		std::string sideNames[] = {"left", "right", "up", "down", "front", "back"};

		unsigned int sideIds[] =
		{
				GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
				GL_TEXTURE_CUBE_MAP_POSITIVE_Y, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
				GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
		};

		for (unsigned int i = 0; i < 6; i++)
		{
			const auto fullPath = FileSystem::ToString(folderPath / sideNames[i]) + fileType;

			int width, height, channels;

			const auto data = stbi_load(fullPath.c_str(), &width, &height, &channels, 0);

			if (data)
			{
				glTexImage2D(sideIds[i], 0, GL_RGBA, width, height, 0, channels == 3 ? GL_RGB : GL_RGBA, GL_UNSIGNED_BYTE, data);
				stbi_image_free(data);

				std::cout << "TextureManager::GetTextureCubemap - Loaded image: " << fullPath.c_str() << "\n";
			}
			else
			{
				stbi_image_free(data);

				std::cout << "TextureManager::GetTextureCubemap - Failed to load image: " << fullPath.c_str() << "\n";
				glDeleteTextures(1, &texture->id);
				return nullptr;
			}
		}

		std::cout << "TextureManager::GetTextureCubemap - Loaded cubemap, id:   " << texture << "\n";

		// Save and return the id
		m_Textures[path] = texture;
		return texture;
	}

}
