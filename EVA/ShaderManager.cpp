#include "ShaderManager.hpp"

#include <vector>
#include <fstream>
#include <iostream>

#include "EVA/OpenGL.hpp"
#include "Parsers/ShaderParser.hpp"

namespace EVA
{
	std::map<FS::path, std::shared_ptr<Shader>> ShaderManager::m_Shaders;

	std::shared_ptr<Shader> ShaderManager::LoadShader(const FS::path& path)
	{
		std::cout << "ShaderManager::SaveShader - " << FileSystem::ToString(path).c_str() << std::endl;

		if (m_Shaders.count(path))
			return m_Shaders[path];

		const auto pathList = ShaderParser::LoadShader(path);
		if (pathList == nullptr)
			return nullptr;

		const auto shader = std::make_shared<Shader>(pathList);

		m_Shaders[path] = shader;
		return shader;
	}

	void ShaderManager::SaveShader(Shader* shader, const FS::path& path)
	{
		std::cout << "ShaderManager::SaveShader - " << FileSystem::ToString(path).c_str() << std::endl;

		Json::Document d;
		d.SetObject();

		auto& a = d.GetAllocator();

		DataObject data(d, &a);

		if (!shader->paths->vertex.empty())
			data.Set("vertex", FileSystem::ToString(shader->paths->vertex));

		if (!shader->paths->fragment.empty())
			data.Set("fragment", FileSystem::ToString(shader->paths->fragment));

		if (!shader->paths->geometry.empty())
			data.Set("geometry", FileSystem::ToString(shader->paths->geometry));

		if (!shader->paths->tessControl.empty())
			data.Set("tessControl", FileSystem::ToString(shader->paths->tessControl));

		if (!shader->paths->tessEvaluation.empty())
			data.Set("tessEvaluation", FileSystem::ToString(shader->paths->tessEvaluation));

		if (!shader->paths->compute.empty())
			data.Set("compute", FileSystem::ToString(shader->paths->compute));

		Json::Save(&d, path);
	}

	bool ShaderManager::ReadShaderSource(const FS::path& path, std::vector<char> &buffer)
	{
		std::cout << "ShaderManager::ReadShaderSource - " << FileSystem::ToString(path).c_str() << std::endl;

		std::ifstream in;
		in.open(FileSystem::ToString(path).c_str(), std::ios::binary);

		if (!in.fail())
		{
			// Get the number of bytes stored in this file
			in.seekg(0, std::ios::end);
			const auto length = (size_t) in.tellg();

			// Go to start of the file
			in.seekg(0, std::ios::beg);

			// Read the content of the file in a buffer
			buffer.resize(length + 1);
			in.read(&buffer[0], length);
			in.close();

			// Add a valid C-string end
			buffer[length] = '\0';
		} 
		else
		{
			std::cout << "ShaderManager::ReadShaderSource - Unable to open file: " << FileSystem::ToString(path).c_str() << std::endl;
			return false;
		}

		return true;
	}

	int ShaderManager::LoadAndCompileShader(const FS::path& path, const GLenum shaderType)
	{
		std::cout << "ShaderManager::LoadAndCompileShader - " << FileSystem::ToString(path).c_str() << std::endl;

		// Load a shader from an external file
		std::vector<char> buffer;
		if(!ReadShaderSource(path, buffer))
		{
			return -1;
		}

		const char *src = &buffer[0];

		// Create shaders
		GLCall(const auto shader = glCreateShader(shaderType));

		//Attach the shader source code to the shader object
		GLCall(glShaderSource(shader, 1, &src, nullptr));

		// Compile the shader
		GLCall(glCompileShader(shader));

		// Comile the shader, translates into internal representation and checks for errors.
		GLint compileOk;
		GLCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &compileOk));
		if (!compileOk)
		{
			char infolog[1024];;
			GLCall(glGetShaderInfoLog(shader, 1024, nullptr, infolog));
			std::cout << "ShaderManager::LoadAndCompileShader - The program failed to compile with the error:" << std::endl << infolog << std::endl;

			GLCall(glDeleteShader(shader));
			return -1;
		}

		std::cout << "ShaderManager::LoadAndCompileShader - Successfully compiled shader" << std::endl;
		return shader;
	}
}
