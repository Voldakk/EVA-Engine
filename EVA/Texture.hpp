#pragma once

#include "EVA/OpenGL.hpp"
#include "stb_image.h"

namespace EVA
{
	enum class TextureTarget
	{
		Texture1D = GL_TEXTURE_1D,
		Texture2D = GL_TEXTURE_2D,
		Texture3D = GL_TEXTURE_3D,
		Texture1DArray = GL_TEXTURE_1D_ARRAY,
		Texture2DArray = GL_TEXTURE_2D_ARRAY,
		TextureRectangle = GL_TEXTURE_RECTANGLE,
		TextureCubeMap = GL_TEXTURE_CUBE_MAP,
		TextureCubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
		TextureBuffer = GL_TEXTURE_BUFFER,
		Texture2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
		Texture2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY
	};

	enum class TextureWrapping
	{
		ClampToEdge = GL_CLAMP_TO_EDGE,
		Repeat = GL_REPEAT,
		MirroredRepeat = GL_MIRRORED_REPEAT

	};

	enum class TextureMinFilter
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR,
		NearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		LinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		NearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
		LinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
	};

	enum class TextureMagFilter
	{
		Nearest = GL_NEAREST,
		Linear = GL_LINEAR
	};

	enum class TextureFormat
	{
		R8 = GL_R8,
		R8_SNORM = GL_R8_SNORM,
		R16 = GL_R16,
		R16_SNORM = GL_R16_SNORM,
		RG8 = GL_RG8,
		RG8_SNORM = GL_RG8_SNORM,
		RG16 = GL_RG16,
		RG16_SNORM = GL_RG16_SNORM,
		R3_G3_B2 = GL_R3_G3_B2,
		RGB4 = GL_RGB4,
		RGB5 = GL_RGB5,
		RGB8 = GL_RGB8,
		RGB8_SNORM = GL_RGB8_SNORM,
		RGB10 = GL_RGB10,
		RGB12 = GL_RGB12,
		RGB16_SNORM = GL_RGB16_SNORM,
		RGBA2 = GL_RGBA2,
		RGBA4 = GL_RGBA4,
		RGB5_A1 = GL_RGB5_A1,
		RGBA8 = GL_RGBA8,
		RGBA8_SNORM = GL_RGBA8_SNORM,
		RGB10_A2 = GL_RGB10_A2,
		RGB10_A2UI = GL_RGB10_A2UI,
		RGBA12 = GL_RGBA12,
		RGBA16 = GL_RGBA16,
		SRGB8 = GL_SRGB8,
		SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
		R16F = GL_R16F,
		RG16F = GL_RG16F,
		RGB16F = GL_RGB16F,
		RGBA16F = GL_RGBA16F,
		R32F = GL_R32F,
		RG32F = GL_RG32F,
		RGB32F = GL_RGB32F,
		RGBA32F = GL_RGBA32F,
		R11F_G11F_B10F = GL_R11F_G11F_B10F,
		RGB9_E5 = GL_RGB9_E5,
		R8I = GL_R8I,
		R8UI = GL_R8UI,
		R16I = GL_R16I,
		R16UI = GL_R16UI,
		R32I = GL_R32I,
		R32UI = GL_R32UI,
		RG8I = GL_RG8I,
		RG8UI = GL_RG8UI,
		RG16I = GL_RG16I,
		RG16UI = GL_RG16UI,
		RG32I = GL_RG32I,
		RG32UI = GL_RG32UI,
		RGB8I = GL_RGB8I,
		RGB8UI = GL_RGB8UI,
		RGB16I = GL_RGB16I,
		RGB16UI = GL_RGB16UI,
		RGB32I = GL_RGB32I,
		RGB32UI = GL_RGB32UI,
		RGBA8I = GL_RGBA8I,
		RGBA8UI = GL_RGBA8UI,
		RGBA16I = GL_RGBA16I,
		RGBA16UI = GL_RGBA16UI,
		RGBA32I = GL_RGBA32I,
		RGBA32UI = GL_RGBA32UI,
		RED = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
	};

	enum class TextureDataType
	{
		UnsignedByte = GL_UNSIGNED_BYTE,
		Byte = GL_BYTE,
		UnsignedShort = GL_UNSIGNED_SHORT,
		Short = GL_SHORT,
		UnsignedInt = GL_UNSIGNED_INT,
		Int = GL_INT,
		HalfFloat = GL_HALF_FLOAT,
		Float = GL_FLOAT,
		UnsignedByte332 = GL_UNSIGNED_BYTE_3_3_2,
		UnsignedByte233Rev = GL_UNSIGNED_BYTE_2_3_3_REV,
		UnsignedShort565 = GL_UNSIGNED_SHORT_5_6_5,
		UnsignedShort565Rev = GL_UNSIGNED_SHORT_5_6_5_REV,
		UnsignedShort4444 = GL_UNSIGNED_SHORT_4_4_4_4,
		UnsignedShort4444Rev = GL_UNSIGNED_SHORT_4_4_4_4_REV,
		UnsignedShort5551 = GL_UNSIGNED_SHORT_5_5_5_1,
		UnsignedShort1555Rev = GL_UNSIGNED_SHORT_1_5_5_5_REV,
		UnsignedInt8888 = GL_UNSIGNED_INT_8_8_8_8,
		UnsignedInt8888Rev = GL_UNSIGNED_INT_8_8_8_8_REV,
		UnsignedInt1010102 = GL_UNSIGNED_INT_10_10_10_2,
		UnsignedInt2101010Rev = GL_UNSIGNED_INT_2_10_10_10_REV
	};

	// Struct for holding texture info
	struct Texture
	{
		unsigned int id{};
		unsigned int width;
		unsigned int height;
		TextureTarget target;
		TextureWrapping wrapping;
		TextureMinFilter minFilter;
		TextureMagFilter magFilter;
		TextureFormat format;
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

	inline TextureFormat GetTextureFormat(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RED:
		case TextureFormat::R8:
		case TextureFormat::R8_SNORM:
		case TextureFormat::R16:
		case TextureFormat::R16_SNORM:
		case TextureFormat::R16F:
		case TextureFormat::R32F:
		case TextureFormat::R8I:
		case TextureFormat::R8UI:
		case TextureFormat::R16I:
		case TextureFormat::R16UI:
		case TextureFormat::R32I:
		case TextureFormat::R32UI:
			return TextureFormat::RED;

		case TextureFormat::RG:
		case TextureFormat::RG8:
		case TextureFormat::RG8_SNORM:
		case TextureFormat::RG16:
		case TextureFormat::RG16_SNORM:
		case TextureFormat::RG16F:
		case TextureFormat::RG32F:
		case TextureFormat::RG8I:
		case TextureFormat::RG8UI:
		case TextureFormat::RG16I:
		case TextureFormat::RG16UI:
		case TextureFormat::RG32I:
		case TextureFormat::RG32UI:
			return TextureFormat::RG;

		case TextureFormat::RGB:
		case TextureFormat::R3_G3_B2:
		case TextureFormat::RGB4:
		case TextureFormat::RGB5:
		case TextureFormat::RGB8:
		case TextureFormat::RGB8_SNORM:
		case TextureFormat::RGB10:
		case TextureFormat::RGB12:
		case TextureFormat::RGB16_SNORM:
		case TextureFormat::RGBA2:
		case TextureFormat::RGBA4:
		case TextureFormat::SRGB8:
		case TextureFormat::RGB16F:
		case TextureFormat::RGB32F:
		case TextureFormat::R11F_G11F_B10F:
		case TextureFormat::RGB9_E5:
		case TextureFormat::RGB8I:
		case TextureFormat::RGB8UI:
		case TextureFormat::RGB16I:
		case TextureFormat::RGB16UI:
		case TextureFormat::RGB32I:
		case TextureFormat::RGB32UI:
			return TextureFormat::RGB;

		case TextureFormat::RGBA:
		case TextureFormat::RGB5_A1:
		case TextureFormat::RGBA8:
		case TextureFormat::RGBA8_SNORM:
		case TextureFormat::RGB10_A2:
		case TextureFormat::RGB10_A2UI:
		case TextureFormat::RGBA12:
		case TextureFormat::RGBA16:
		case TextureFormat::SRGB8_ALPHA8:
		case TextureFormat::RGBA16F:
		case TextureFormat::RGBA32F:
		case TextureFormat::RGBA8I:
		case TextureFormat::RGBA8UI:
		case TextureFormat::RGBA16I:
		case TextureFormat::RGBA16UI:
		case TextureFormat::RGBA32I:
		case TextureFormat::RGBA32UI:
			return TextureFormat::RGBA;

		default:
			throw;
		}
	}

	inline TextureDataType GetTextureDataType(TextureFormat format)
	{
		switch (format)
		{
		case TextureFormat::RED:
		case TextureFormat::RG:
		case TextureFormat::RGB:
		case TextureFormat::RGBA:
		case TextureFormat::R8:
		case TextureFormat::RG8:
		case TextureFormat::RGB8:
		case TextureFormat::RGBA8:
		case TextureFormat::R8UI:
		case TextureFormat::RG8UI:
		case TextureFormat::RGB8UI:
		case TextureFormat::RGBA8UI:
		case TextureFormat::SRGB8:
		case TextureFormat::SRGB8_ALPHA8:
			return TextureDataType::UnsignedByte;

		case TextureFormat::R8I:
		case TextureFormat::RG8I:
		case TextureFormat::RGB8I:
		case TextureFormat::RGBA8I:
		case TextureFormat::R8_SNORM:
		case TextureFormat::RG8_SNORM:
		case TextureFormat::RGB8_SNORM:
		case TextureFormat::RGBA8_SNORM:
			return TextureDataType::Byte;

		case TextureFormat::R16:
		case TextureFormat::RG16:
		case TextureFormat::RGB16_SNORM:
		case TextureFormat::RGBA16:
		case TextureFormat::R16UI:
		case TextureFormat::RG16UI:
		case TextureFormat::RGB16UI:
		case TextureFormat::RGBA16UI:
			return TextureDataType::UnsignedShort;

		case TextureFormat::R16I:
		case TextureFormat::RG16I:
		case TextureFormat::RGB16I:
		case TextureFormat::RGBA16I:
		case TextureFormat::R16_SNORM:
		case TextureFormat::RG16_SNORM:
			return TextureDataType::Short;

		case TextureFormat::R32I:
		case TextureFormat::RG32I:
		case TextureFormat::RGB32I:
		case TextureFormat::RGBA32I:
			return TextureDataType::Int;

		case TextureFormat::R32UI:
		case TextureFormat::RG32UI:
		case TextureFormat::RGB32UI:
		case TextureFormat::RGBA32UI:
			return TextureDataType::UnsignedInt;

		case TextureFormat::R16F:
		case TextureFormat::RG16F:
		case TextureFormat::RGB16F:
		case TextureFormat::RGBA16F:
			//return TextureDataType::HalfFloat;

		case TextureFormat::R32F:
		case TextureFormat::RG32F:
		case TextureFormat::RGB32F:
		case TextureFormat::RGBA32F:
			return TextureDataType::Float;

		case TextureFormat::R3_G3_B2:
			return TextureDataType::UnsignedByte332;

		case TextureFormat::RGB5_A1:
			return TextureDataType::UnsignedShort5551;

		case TextureFormat::RGB10_A2:
		case TextureFormat::RGB10_A2UI:
			return TextureDataType::UnsignedInt1010102;

		case TextureFormat::RGB4:
		case TextureFormat::RGB5:
		case TextureFormat::RGB10:
		case TextureFormat::RGB12:
		case TextureFormat::RGBA2:
		case TextureFormat::RGBA4:
		case TextureFormat::R11F_G11F_B10F:
		case TextureFormat::RGB9_E5:
		case TextureFormat::RGBA12:

		default:
			throw;
		}
	}
}
