#pragma once
#include <memory>

#include "Json.hpp"
#include "../Shader.hpp"

namespace EVA
{
	class EVA_API ShaderParser
	{
	public:

		static std::shared_ptr<ShaderPaths> LoadShader(const FS::path& path)
		{
			auto shaderPaths = std::make_shared<ShaderPaths>();

			shaderPaths->shader = path;

			const auto sd = Json::Open(path);

			if (sd == nullptr)
			{
				std::cout << "ShaderParser::LoadShader - Invalid shader file: " << FileSystem::ToString(path) << std::endl;
				return nullptr;
			}

			auto& d = (*sd);

			if (!d.IsObject())
			{
				std::cout << "ShaderParser::LoadShader - Invalid shader file: " << FileSystem::ToString(path) << std::endl;
				return nullptr;
			}

			DataObject data(d);

			shaderPaths->vertex = data.Get<FS::path>("vertex", "");
			shaderPaths->fragment = data.Get<FS::path>("fragment", "");
			shaderPaths->geometry = data.Get<FS::path>("geometry", "");
			shaderPaths->tessControl = data.Get<FS::path>("tessControl", "");
			shaderPaths->tessEvaluation = data.Get<FS::path>("tessEvaluation", "");

			return shaderPaths;
		}
	};
}
