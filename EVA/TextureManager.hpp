#pragma once

#include <map>

#include "FileSystem.hpp"
#include "stb_image.h"

#include "EVA/OpenGL.hpp"

namespace EVA
{
	// Different texture types
	enum class TextureType
	{
		Diffuse, Specular, Normal, Emission, Height
	};

	enum class TextureWrapping : GLint
	{
		ClampToEdge = GL_CLAMP_TO_EDGE,
		Repeat = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT

	};

	enum class TextureMinFilter : GLint
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR,
		NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
		LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
	};

	enum class TextureMagFilter : GLint
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR
	};

	// Struct for holding texture info
	struct Texture
	{
		unsigned int id{};
		TextureType type;
		FS::path path;
	};

	struct RawTexture
	{
		unsigned char* data;
		unsigned int width;
		unsigned int height;
		unsigned int channels;

		RawTexture(unsigned char* data, const unsigned int width, const unsigned int height, const unsigned int channels)
		{
			this->data = data;
			this->width = width;
			this->height = height;
			this->channels = channels;
		}

		~RawTexture()
		{
			stbi_image_free(data);
		}
	};

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
		static std::shared_ptr<Texture> LoadTextureCubemap(const FS::path& folderPath, const std::string &fileType);

		static std::shared_ptr<RawTexture> LoadRaw(const FS::path& path);
	};

}
