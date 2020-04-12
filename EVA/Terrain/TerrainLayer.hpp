#pragma once

#include "EVA.hpp"
#include "../Materials/PBRMaterial.hpp"
#include "../MaterialManager.hpp"

namespace EVA
{
	class TerrainLayer : public ISerializeable
	{
	public:

		std::shared_ptr<PBRMaterial> material;
		std::shared_ptr<Range<float>> height;
		std::shared_ptr<Range<float>> angle;

		TerrainLayer() = default;

		// Inherited via ISerializeable
		virtual void Serialize(DataObject& data) override
		{
			auto materialPath = material != nullptr ? material->path : "";
			if (data.Serialize("material", materialPath))
				material = std::dynamic_pointer_cast<PBRMaterial>(MaterialManager::LoadMaterial(materialPath));

			data.Serialize("height", height);
			data.Serialize("angle", angle);
		}
	};

}