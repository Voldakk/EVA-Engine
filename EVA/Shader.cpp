#include "Shader.hpp"

#include "EVA/OpenGL.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "ShaderManager.hpp"
#include <iostream>

namespace EVA
{
	Shader::Shader()
	{
		SetPaths(std::make_shared<ShaderPaths>());
	}

	Shader::Shader(const std::shared_ptr<ShaderPaths>& paths)
	{
		SetPaths(paths);
	}

	Shader::~Shader()
	{
		GLCall(glDeleteProgram(m_ShaderId));
	}

	void Shader::SetPaths(const std::shared_ptr<ShaderPaths>& paths)
	{
		if (m_ShaderId != -1) 
		{
			GLCall(glDeleteProgram(m_ShaderId));
		}

		m_Paths = paths;

		GLCall(m_ShaderId = glCreateProgram());

		if (!m_Paths->vertex.empty())
		{
			const auto vertexShader = ShaderManager::LoadAndCompileShader(m_Paths->vertex, GL_VERTEX_SHADER);
			if (vertexShader != -1)
			{
				GLCall(glAttachShader(m_ShaderId, vertexShader));
				GLCall(glDeleteShader(vertexShader));
			}
			else
				m_Paths->vertex = "";
		}

		if (!m_Paths->fragment.empty())
		{
			const auto fragmentShader = ShaderManager::LoadAndCompileShader(m_Paths->fragment, GL_FRAGMENT_SHADER);
			if (fragmentShader != -1)
			{
				GLCall(glAttachShader(m_ShaderId, fragmentShader));
				GLCall(glDeleteShader(fragmentShader));
			}
			else
				m_Paths->fragment = "";
		}

		if (!m_Paths->geometry.empty())
		{
			const auto geometryShader = ShaderManager::LoadAndCompileShader(m_Paths->geometry, GL_GEOMETRY_SHADER);
			if (geometryShader != -1)
			{
				GLCall(glAttachShader(m_ShaderId, geometryShader));
				GLCall(glDeleteShader(geometryShader));
			}
			else
				m_Paths->geometry = "";
		}

		if (!m_Paths->tessControl.empty())
		{
			const auto tessControlShader = ShaderManager::LoadAndCompileShader(m_Paths->tessControl, GL_TESS_CONTROL_SHADER);
			if (tessControlShader != -1)
			{
				GLCall(glAttachShader(m_ShaderId, tessControlShader));
				GLCall(glDeleteShader(tessControlShader));
			}
			else
				m_Paths->tessControl = "";
		}

		if (!m_Paths->tessEvaluation.empty())
		{
			const auto tessellationEvaluationShader = ShaderManager::LoadAndCompileShader(m_Paths->tessEvaluation, GL_TESS_EVALUATION_SHADER);
			if (tessellationEvaluationShader != -1)
			{
				GLCall(glAttachShader(m_ShaderId, tessellationEvaluationShader));
				GLCall(glDeleteShader(tessellationEvaluationShader));
			}
			else
				m_Paths->tessEvaluation = "";
		}

		if (!m_Paths->compute.empty())
		{
			const auto computeShader = ShaderManager::LoadAndCompileShader(m_Paths->compute, GL_COMPUTE_SHADER);
			if (computeShader != -1)
			{
				GLCall(glAttachShader(m_ShaderId, computeShader));
				GLCall(glDeleteShader(computeShader));
			}
			else
				m_Paths->compute = "";
		}

		GLCall(glLinkProgram(m_ShaderId));
		GLint linkOk;
		GLCall(glGetProgramiv(m_ShaderId, GL_LINK_STATUS, &linkOk));
		if (!linkOk)
		{
			char infolog[1024];;
			GLCall(glGetShaderInfoLog(m_ShaderId, 1024, nullptr, infolog));
			std::cout << "The program failed to link with the error:" << std::endl << infolog << std::endl;

			GLCall(glDeleteShader(m_ShaderId));
		}

		glValidateProgram(m_ShaderId);
		GLint validateOk;
		GLCall(glGetProgramiv(m_ShaderId, GL_VALIDATE_STATUS, &validateOk));
		if (!validateOk)
		{
			char infolog[1024];;
			GLCall(glGetShaderInfoLog(m_ShaderId, 1024, nullptr, infolog));
			std::cout << "The program failed to validate with the error:" << std::endl << infolog << std::endl;

			GLCall(glDeleteShader(m_ShaderId));
		}

		GLCall(glUseProgram(m_ShaderId));
	}

	void Shader::Bind() const
	{
		GLCall(glUseProgram(m_ShaderId));
	}

	void Shader::Unbind()
	{
		GLCall(glUseProgram(0));
	}

	int Shader::GetUniformLocation(const std::string& name)
	{
		if (m_UniformLocationMap.find(name) != m_UniformLocationMap.end())
			return m_UniformLocationMap[name];

		GLCall(const auto location = glGetUniformLocation(m_ShaderId, name.c_str()));
		m_UniformLocationMap[name] = location;
		return location;
	}

	void Shader::SetUniform1I(const std::string& name, const int value)
	{
		GLCall(glUniform1i(GetUniformLocation(name), value));
	}

	void Shader::SetUniform1F(const std::string& name, const float value)
	{
		GLCall(glUniform1f(GetUniformLocation(name), value));
	}

	void Shader::SetUniform2Fv(const std::string& name, glm::vec2 value)
	{
		GLCall(glUniform2fv(GetUniformLocation(name), 1, value_ptr(value)));
	}

	void Shader::SetUniform3Fv(const std::string& name, glm::vec3 value)
	{
		GLCall(glUniform3fv(GetUniformLocation(name), 1, value_ptr(value)));
	}

	void Shader::SetUniform4Fv(const std::string& name, glm::vec4 value)
	{
		GLCall(glUniform4fv(GetUniformLocation(name), 1, value_ptr(value)));
	}

	void Shader::SetUniformMatrix4Fv(const std::string& name, glm::mat4 value)
	{
		GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, value_ptr(value)));
	}

	void Shader::BindTexture(std::shared_ptr<Texture> texture, const std::string& name, const int unit)
	{
		SetUniform1I(name, unit);
		GLCall(glActiveTexture(GL_TEXTURE0 + unit));

		if (texture != nullptr) 
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, texture->id));
		}
		else 
		{
			GLCall(glBindTexture(GL_TEXTURE_2D, 0));
		}
	}

	void Shader::BindImageTexture(std::shared_ptr<Texture> texture, const ImageAccess access, const int unit, const int level, const bool layered, const int layer)
	{
		GLCall(glBindImageTexture(unit, texture->id, level, layered, layer, (GLenum)access, (GLenum)texture->format));
	}
	void Shader::DispatchCompute(unsigned int numGroupsX, unsigned int numGroupsY, unsigned int numGroupsZ)
	{
		GLCall(glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ));
	}
}
