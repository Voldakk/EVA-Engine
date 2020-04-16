#pragma once

#include <charconv>
#include <string.h>

#include "imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../FileSystem.hpp"

namespace EVA
{
	class ISerializeable;

	class InspectorFields
	{
	public:

		inline static const int STRING_LENGTH = 10000;
		inline static char* C_STRING = new char[STRING_LENGTH];

		inline static const float INDENT = 10.0f;

		static char* GetCString(const std::string& value);

		static void Text(std::string text);

		static void Text(const char* text);

		static bool Int(const char* name, int& value);

		static bool EnterInt(const char* name, int& value);

		static bool Bool(const char* name, bool& value);

		static bool Float(const char* name, float& value);

		static bool Float2(const char* name, glm::vec2& value);

		static bool Float3(const char* name, glm::vec3& value);

		static bool Float4(const char* name, glm::vec4& value);

		static bool String(const char* name, std::string& value);

		static bool EnterString(const char* name, std::string& value);

		static bool DragDropTargetString(const char* name, std::string& value, const char* payloadType);

		static bool ColorPicker(const char* name, glm::vec3& value);

		static bool ColorPicker(const char* name, glm::vec4& value);

		static bool DragFloat(const char* name, float& value, const float min = 0.0f, const float max = 1.0f, const float step = 0.01f);

		static bool RangeFloat(const char* name, float& min, float& max, const float step = 0.01f);

		static bool Button(const char* name);

		static bool Path(const char* name, FS::path& value);

		static bool Serializeable(const char* name, const std::shared_ptr<ISerializeable>& value);

		// Templates

		template <typename T>
		static bool Vector(const char* name, std::vector<T>& value);

		template <typename T>
		static bool Default(const char* name, T& value);

		template <>
		static bool Default(const char* name, int& value);

		template <>
		static bool Default(const char* name, bool& value);

		template <>
		static bool Default(const char* name, std::string& value);

		template <typename T, typename Alloc>
		static bool Default(const char* name, std::vector<T, Alloc>& value);

		template <>
		static bool Default(const char* name, float& value);

		template <>
		static bool Default(const char* name, glm::vec2& value);

		template <>
		static bool Default(const char* name, glm::vec3& value);

		template <>
		static bool Default(const char* name, glm::vec4& value);

		template <>
		static bool Default(const char* name, glm::quat& value);

		template <>
		static bool Default(const char* name, FS::path& value);

		template <typename T = typename ISerializeable>
		static bool Default(const char* name, std::shared_ptr<T>& value);
	};

	template<typename T>
	inline bool InspectorFields::Vector(const char* name, std::vector<T>& value)
	{
		bool changed = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent(INDENT);
			for (size_t i = 0; i < value.size(); i++)
			{
				changed = changed || Default((std::to_string(i) + "##" + name).c_str(), value[i]);
			}
			ImGui::Unindent(INDENT);
		}
		return changed;
	}

	template<typename T>
	inline bool InspectorFields::Default(const char* name, T& value)
	{
		return false;
	}

	template<>
	inline bool InspectorFields::Default(const char* name, int& value)
	{
		return Int(name, value);
	}

	template<>
	inline bool InspectorFields::Default(const char* name, bool& value)
	{
		return Bool(name, value);
	}

	template<>
	inline bool InspectorFields::Default(const char* name, std::string& value)
	{
		return EnterString(name, value);
	}

	template<typename T, typename Alloc>
	inline bool InspectorFields::Default(const char* name, std::vector<T, Alloc>& value)
	{
		return Vector(name, value);
	}

	template<>
	inline bool InspectorFields::Default(const char* name, float& value)
	{
		return Float(name, value);
	}

	template<>
	inline bool InspectorFields::Default(const char* name, glm::vec2& value)
	{
		return Float2(name, value);
	}

	template<>
	inline bool InspectorFields::Default(const char* name, glm::vec3& value)
	{
		return Float3(name, value);
	}

	template<>
	inline bool InspectorFields::Default(const char* name, glm::vec4& value)
	{
		return Float4(name, value);
	}

	template<>
	inline bool InspectorFields::Default(const char* name, glm::quat& value)
	{
		auto vec4 = glm::vec4(value.x, value.y, value.z, value.w);
		bool changed = Float4(name, vec4);

		value = glm::quat(vec4.w, vec4.x, vec4.y, vec4.z);

		return changed;
	}

	template<>
	inline bool InspectorFields::Default(const char* name, FS::path& value)
	{
		auto pathString = FileSystem::ToString(value);
		bool changed = DragDropTargetString(name, pathString, "file");

		value = pathString;

		return changed;
	}

	template<typename T>
	inline bool InspectorFields::Default(const char* name, std::shared_ptr<T>& value)
	{
		if (value == nullptr)
			return false;

		return Serializeable(name, std::dynamic_pointer_cast<ISerializeable>(value));
	}
}
