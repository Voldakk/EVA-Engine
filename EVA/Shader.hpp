#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "FileSystem.hpp"
#include "TextureManager.hpp"

namespace EVA
{
	struct ShaderPaths
	{
		FS::path shader;
		FS::path vertex;
		FS::path fragment;
		FS::path geometry;
		FS::path tessControl;
		FS::path tessEvaluation;
		FS::path compute;
	};

	enum class ImageAccess : GLenum
	{
		ReadOnly = GL_READ_ONLY,
		WriteOnly = GL_WRITE_ONLY,
		ReadWrite = GL_READ_WRITE
	};

	/**
	 * \brief Used to interact with an OpenGL shader program
	 */
	class Shader
	{
		int m_ShaderId = -1;
		std::unordered_map<std::string, int> m_UniformLocationMap;
		std::shared_ptr<ShaderPaths> m_Paths;

	public:

		std::shared_ptr<ShaderPaths>& paths = m_Paths;

		Shader();
		explicit Shader(const std::shared_ptr<ShaderPaths>& paths);

		~Shader();
		
		void SetPaths(const std::shared_ptr<ShaderPaths>& paths);

		void Bind() const;

		static void Unbind();

		void SetUniform1I(const std::string& name, int value);

		void SetUniform1F(const std::string& name, float value);

		void SetUniform2Fv(const std::string& name, glm::vec2 value);

		void SetUniform3Fv(const std::string& name, glm::vec3 value);
		
		void SetUniform4Fv(const std::string& name, glm::vec4 value);

		void SetUniformMatrix4Fv(const std::string& name, glm::mat4 value);

		void BindTexture(std::shared_ptr<Texture> texture, const std::string& name, const int unit = 0);
		void BindImageTexture(std::shared_ptr<Texture> texture, const ImageAccess access, const int unit = 0, const int level = 0, const bool layered = false, const int layer = 0);

		void DispatchCompute(unsigned int numGroupsX, unsigned int numGroupsY, unsigned int numGroupsZ = 1);

	private:

		int GetUniformLocation(const std::string& name);

	};
}
