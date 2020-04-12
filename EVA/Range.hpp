#pragma once

#include "Parsers/Json.hpp"

namespace EVA
{
	template <typename T>
	class Range : public ISerializeable
	{
	public:

		T min;
		T max;

		Range() = default;
		virtual ~Range() = default;
		Range(T min, T max) : min(min), max(max) {}

		bool Contains(T value)
		{
			return min <= value && value <= max;
		}

		// Inherited via ISerializeable
		virtual void Serialize(DataObject& data) override
		{
			data.Serialize("min", min);
			data.Serialize("max", max);
		}
	};
}