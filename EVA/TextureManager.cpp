#include "TextureManager.hpp"

#include <iostream>

#include "EVA/OpenGL.hpp"

namespace EVA
{
	std::map<FS::path, std::shared_ptr<Texture>> TextureManager::m_Textures;

	std::shared_ptr<Texture> TextureManager::LoadTexture(const FS::path& path, const TextureWrapping wrapping, const TextureMinFilter minFilter, const TextureMagFilter magFilter)
	{
		// Return the id if the texture's already loaded
		if (m_Textures.count(path))
			return m_Textures[path];

		std::cout << "TextureManager::LoadTexture - Loading texture: " << FileSystem::ToString(path) << "\n";

		auto texture = std::make_shared<Texture>();
		texture->wrapping = wrapping;
		texture->minFilter = minFilter;
		texture->magFilter = magFilter;
		texture->path = path;

		// Load the image
		stbi_set_flip_vertically_on_load(true);

		int width, height, channels;
		const auto data = stbi_load(FileSystem::ToString(path).c_str(), &width, &height, &channels, 0);


		// If the image was loaded
		if (data)
		{
			texture->width = width;
			texture->height = height;

			TextureFormat format;
			switch (channels)
			{
			case 1:
				format = TextureFormat::RED;
				break;
			case 2:
				format = TextureFormat::RG;
				break;
			case 3:
				format = TextureFormat::RGB;
				break;
			default:
				format = TextureFormat::RGBA;
				break;
			}
			texture->format = format;

			// Create texture
			GLCall(glGenTextures(1, &texture->id));
			GLCall(glBindTexture(GL_TEXTURE_2D, texture->id));

			// Texture parameters
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrapping));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrapping));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)minFilter));
			GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)magFilter));

			// Save the texture
			GLCall(glTexImage2D(GL_TEXTURE_2D, 0, (GLenum)texture->format, width, height, 0, (GLenum)texture->format, GL_UNSIGNED_BYTE, data));
			GLCall(glGenerateMipmap(GL_TEXTURE_2D));

			// Save the id
			m_Textures[path] = texture;
			
			
			std::cout << "TextureManager::LoadTexture - Texture id:   " << texture->id << "\n";

			stbi_image_free(data);
			return texture;
		}
		else // If not
		{
			std::cout << "TextureManager::LoadTexture - Failed to load image: " << FileSystem::ToString(path) << "\n";
			return nullptr;
		}
	}

	std::shared_ptr<Texture> TextureManager::LoadTextureCubemap(const FS::path& folderPath, const std::string &fileType, const TextureWrapping wrapping, const TextureMinFilter minFilter, const TextureMagFilter magFilter)
	{
		if (folderPath.empty())
			return nullptr;

		const auto path = folderPath / fileType;

		// Return the id if the texture's already loaded
		if (m_Textures.count(path))
			return m_Textures[path];

		auto texture = std::make_shared<Texture>();
		texture->wrapping = wrapping;
		texture->minFilter = minFilter;
		texture->magFilter = magFilter;
		texture->path = path;

		// Create texture
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glGenTextures(1, &texture->id));
		GLCall(glBindTexture(GL_TEXTURE_CUBE_MAP, texture->id));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, (GLint)magFilter));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, (GLint)minFilter));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, (GLint)wrapping));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, (GLint)wrapping));
		GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, (GLint)wrapping));

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
				texture->width = width;
				texture->height = height;
				texture->format = channels == 3 ? TextureFormat::RGB : TextureFormat::RGBA;
				GLCall(glTexImage2D(sideIds[i], 0, (GLenum)texture->format, width, height, 0, (GLenum)texture->format, GL_UNSIGNED_BYTE, data));
				stbi_image_free(data);

				std::cout << "TextureManager::LoadTextureCubemap - Loaded texture: " << fullPath << "\n";
			}
			else
			{
				stbi_image_free(data);

				std::cout << "TextureManager::LoadTextureCubemap - Failed to load texture: " << fullPath << "\n";
				GLCall(glDeleteTextures(1, &texture->id));
				return nullptr;
			}
		}

		std::cout << "TextureManager::LoadTextureCubemap - Loaded cubemap, id: " << texture->id << "\n";

		// Save and return the id
		m_Textures[path] = texture;
		return texture;
	}

	std::shared_ptr<RawTexture> TextureManager::LoadRaw(const FS::path& path)
	{
		// No need to flip is as it's not used as a OpenGL texture
		stbi_set_flip_vertically_on_load(false);

		// Load the image data
		int width, height, channels;
		const auto data = stbi_load(FileSystem::ToString(path).c_str(), &width, &height, &channels, 0);

		// If the image was loaded
		if (data)
		{
			auto texture = std::make_shared<RawTexture>(data, width, height, channels);

			std::cout << "TextureManager::LoadRaw - Loaded image: " << FileSystem::ToString(path) << "\n";
			return texture;
		}
		else // If not
		{
			std::cout << "TextureManager::LoadRaw - Failed to load image: " << FileSystem::ToString(path) << "\n";
			return nullptr;
		}
	}

	std::shared_ptr<Texture> TextureManager::CreateTexture(const int width, const int height, const TextureFormat format, const TextureWrapping wrapping, const TextureMinFilter minFilter, const TextureMagFilter magFilter)
	{
		auto texture = std::make_shared<Texture>();
		texture->width = width;
		texture->height = height;
		texture->wrapping = wrapping;
		texture->minFilter = minFilter;
		texture->magFilter = magFilter;
		texture->format = format;

		// Create texture
		GLCall(glGenTextures(1, &texture->id));
		GLCall(glBindTexture(GL_TEXTURE_2D, texture->id));

		// Texture parameters
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, (GLint)wrapping));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, (GLint)wrapping));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, (GLint)minFilter));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, (GLint)magFilter));

		GLCall(glTexStorage2D(GL_TEXTURE_2D, glm::log(width) / glm::log(2), (GLenum)format, width, height));

		return texture;
	}
}