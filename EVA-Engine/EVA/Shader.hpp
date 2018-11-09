#pragma once

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

#include "Core.hpp"
#include "FileSystem.hpp"

namespace EVA
{
	struct EVA_API ShaderPaths
	{
		FS::path shader;
		FS::path vertex;
		FS::path fragment;
		FS::path geometry;
		FS::path tessControl;
		FS::path tessEvaluation;
	};

	/**
	 * \brief Used to interact with an OpenGL shader program
	 */
	class EVA_API Shader
	{
		int m_ShaderId = -1;
		std::unordered_map<std::string, int> m_UniformLocationMap;
		std::shared_ptr<ShaderPaths> m_Paths;

	public:

		std::shared_ptr<ShaderPaths>& paths = m_Paths;

		Shader();
		explicit Shader(const std::shared_ptr<ShaderPaths>& paths);

		void SetPaths(const std::shared_ptr<ShaderPaths>& paths);

		~Shader();

		void Bind() const;

		static void Unbind();

		void SetUniform1I(const std::string& name, int value);

		void SetUniform1F(const std::string& name, float value);

		void SetUniform2Fv(const std::string& name, glm::vec2 value);

		void SetUniform3Fv(const std::string& name, glm::vec3 value);

		void SetUniform4Fv(const std::string& name, glm::vec4 value);

		void SetUniformMatrix4Fv(const std::string& name, glm::mat4 value);

	private:

		int GetUniformLocation(const std::string& name);
	};
}
