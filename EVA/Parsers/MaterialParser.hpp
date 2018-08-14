#pragma once

#include "Json.hpp"
#include "../Material.hpp"
#include "../MaterialMap.hpp"

namespace EVA
{

	/**
	 * \brief Parser for material files
	 */
	class MaterialParser
	{

	public:

		/**
		 * \brief Loads a material form a material asset file
		 * \param path The path to the file
		 * \return A pointer to the material, or nullptr if no material is found
		 */
		static std::shared_ptr<Material> Load(const FS::path& path)
		{
			const auto sd = Json::Open(path);

			if (sd == nullptr)
				return nullptr;

			auto& d = (*sd);

			DataObject data(d);

			auto material = MaterialMap::Create(data.GetString("id", ""));

			if (material != nullptr)
			{
				material->path = path;
				material->Load(data);
			}

			return material;
		}

		static void Save(const Material* material, const FS::path& path)
		{
			if (material == nullptr || path.empty())
				return;

			Json::Document d;
			d.SetObject();

			auto& a = d.GetAllocator();

			DataObject data(d, &a);

			data.SetString("id", material->GetTypeId());

			material->Save(data);

			Json::Save(&d, path);
		}

		static void Save(const std::shared_ptr<Material>& material, const FS::path& path)
		{
			if(material != nullptr)
				Save(material.get(), path);
		}
	};

}
