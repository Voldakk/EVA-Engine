#include "Shader.hpp"

#include "EVA/OpenGL.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "ShaderManager.hpp"

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

		GLCall(glLinkProgram(m_ShaderId));
		GLCall(glUseProgram(m_ShaderId));
	}

	Shader::~Shader()
	{
		GLCall(glDeleteProgram(m_ShaderId));
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

	void Shader::Bind() const
	{
		GLCall(glUseProgram(m_ShaderId));
	}

	void Shader::Unbind()
	{
		GLCall(glUseProgram(0));
	}
}
