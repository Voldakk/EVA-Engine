#pragma once

namespace EVA
{
	class DataObject;

	class ISerializeable
	{
	public:
		virtual void Serialize(DataObject& data);
	};
}