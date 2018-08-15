#include "Asset.hpp"

namespace EVA
{

	void Asset::SaveAsset(DataObject & data) const
	{
	}

	void Asset::LoadAsset(const DataObject data)
	{
	}

	void Asset::DrawInspector()
	{
		InspectorFields::Text("-- No inspector --");
	}

	std::string Asset::GetTypeId() const
	{
		return std::string();
	}

}
