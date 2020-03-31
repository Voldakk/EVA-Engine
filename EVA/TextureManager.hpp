#pragma once

#include <map>

#include "FileSystem.hpp"

#include "glm/glm.hpp"

#include "Texture.hpp"

namespace EVA
{
	/**
	 * \brief A helper class used to load images from file to OpenGL textures
	 */
	class TextureManager
	{

		static std::map<FS::path, std::shared_ptr<Texture>> m_Textures;

	public:

		/**
		 * \brief Loads an texture from file.
		 * \param path The path to the texture file
		 * \return A pointer to the texture, or nullptr if no texture is found
		 */
		static std::shared_ptr<Texture> LoadTexture(const FS::path& path, 
			const TextureWrapping wrapping = TextureWrapping::Repeat, 
			const TextureMinFilter minFilter = TextureMinFilter::NearestMipmapLinear,
			const TextureMagFilter magFilter = TextureMagFilter::Linear);

		/**
		 * \brief Loads a series of textures from a folder to a cubemap texture.
		 * \param folderPath The path to the folder
		 * \param fileType The texture file type
		 * \return A pointer to the texture, or nullptr if no texture is found
		 */
		static std::shared_ptr<Texture> LoadTextureCubemap(const FS::path& folderPath, const std::string &fileType,
			const TextureWrapping wrapping = TextureWrapping::ClampToEdge,
			const TextureMinFilter minFilter = TextureMinFilter::Linear,
			const TextureMagFilter magFilter = TextureMagFilter::Linear);

		static std::shared_ptr<RawTexture> LoadRaw(const FS::path& path);

		static std::shared_ptr<Texture> CreateTexture(const int width, const int height, 
			const TextureFormat format,
			const TextureWrapping wrapping = TextureWrapping::Repeat,
			const TextureMinFilter minFilter = TextureMinFilter::Linear,
			const TextureMagFilter magFilter = TextureMagFilter::Linear);
	};

}
