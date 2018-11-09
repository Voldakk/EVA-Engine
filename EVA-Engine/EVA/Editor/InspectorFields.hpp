#pragma once

#include "imgui.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "../FileSystem.hpp"

namespace EVA
{
	class EVA_API InspectorFields
	{
	public:

		static const int STRING_LENGTH = 10000;

		static void Text(std::string text)
		{
			ImGui::Text(text.c_str(), nullptr);
		}

		static void Text(const char* text)
		{
			ImGui::Text(text, nullptr);
		}

		static bool Int(const char* name, int& value)
		{
			return ImGui::InputInt(name, &value);
		}

		static bool EnterInt(const char* name, int& value)
		{
			auto temp = value;
			if (ImGui::InputInt(name, &temp, 1, 5, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				value = temp;
				return true;
			}

			return false;
		}

		static bool Bool(const char* name, bool& value)
		{
			return ImGui::Checkbox(name, &value);
		}

		static bool Float(const char* name, float& value)
		{
			return ImGui::InputFloat(name, &value, 0.0f, 0.0f, "%.5f");
		}

		static bool Float2(const char* name, glm::vec2& value)
		{
			return ImGui::InputFloat2(name, value_ptr(value));
		}

		static bool Float3(const char* name, glm::vec3& value)
		{
			return ImGui::InputFloat3(name, value_ptr(value));
		}

		static bool Float4(const char* name, glm::vec4& value)
		{
			return ImGui::InputFloat4(name, value_ptr(value));
		}

		static bool String(const char* name, std::string& value)
		{
			const auto string = new char[STRING_LENGTH];
			strcpy(string, value.c_str());

			bool changed = ImGui::InputText(name, string, STRING_LENGTH);

			value = string;

			delete[] string;

			return changed;
		}

		static bool EnterString(const char* name, std::string& value)
		{
			const auto string = new char[STRING_LENGTH];
			strcpy(string, value.c_str());

			if (ImGui::InputText(name, string, STRING_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				value = string;
				delete[] string;
				return true;
			}

			delete[] string;
			return false;
		}

		static bool DragDropTargetString(const char* name, std::string& value, const char* payloadType)
		{
			const auto string = new char[STRING_LENGTH];
			strcpy(string, value.c_str());

			ImGui::BeginGroup();
			if (ImGui::InputText(name, string, STRING_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
			{
				value = string;
				delete[] string;
				ImGui::EndGroup();
				return true;
			}
			delete[] string;
			if (ImGui::BeginDragDropTarget())
			{
				const auto payload = ImGui::AcceptDragDropPayload(payloadType);
				if (payload)
				{
					const char* path = (char*)payload->Data;
					value = path;

					ImGui::EndDragDropTarget();
					ImGui::EndGroup();
					return true;
				}
				ImGui::EndDragDropTarget();
			}
			ImGui::EndGroup();

			return false;
		}

		static bool ColorPicker(const char* name, glm::vec3& value)
		{
			return ImGui::ColorEdit3(name, value_ptr(value));
		}

		static bool ColorPicker(const char* name, glm::vec4& value)
		{
			return ImGui::ColorEdit4(name, value_ptr(value));
		}

		static bool DragFloat(const char* name, float& value, const float min = 0.0f, const float max = 1.0f,
		                      const float step = 0.01f)
		{
			return ImGui::DragFloat(name, &value, step, min, max);
		}

		static bool RangeFloat(const char* name, float& min, float& max, const float step = 0.01f)
		{
			return ImGui::DragFloatRange2(name, &min, &max, step);
		}

		static bool Button(const char* name)
		{
			return ImGui::Button(name);
		}

		static bool Path(const char* name, FS::path& value)
		{
			auto pathString = FileSystem::ToString(value);
			bool changed = DragDropTargetString(name, pathString, "file");

			value = pathString;

			return changed;
		}

		// Templates

		template <typename T>
		static bool Default(const char* name, T& value)
		{
			return false;
		}

		template <>
		static bool Default<int>(const char* name, int& value)
		{
			return Int(name, value);
		}

		template <>
		static bool Default<bool>(const char* name, bool& value)
		{
			return Bool(name, value);
		}

		template <>
		static bool Default<std::string>(const char* name, std::string& value)
		{
			return EnterString(name, value);
		}

		template <>
		static bool Default<float>(const char* name, float& value)
		{
			return Float(name, value);
		}

		template <>
		static bool Default<glm::vec2>(const char* name, glm::vec2& value)
		{
			return Float2(name, value);
		}

		template <>
		static bool Default<glm::vec3>(const char* name, glm::vec3& value)
		{
			return Float3(name, value);
		}

		template <>
		static bool Default<glm::vec4>(const char* name, glm::vec4& value)
		{
			return Float4(name, value);
		}

		template <>
		static bool Default<glm::quat>(const char* name, glm::quat& value)
		{
			auto vec4 = glm::vec4(value.x, value.y, value.z, value.w);
			bool changed = Float4(name, vec4);

			value = glm::quat(vec4.w, vec4.x, vec4.y, vec4.z);

			return changed;
		}

		template <>
		static bool Default<FS::path>(const char* name, FS::path& value)
		{
			auto pathString = FileSystem::ToString(value);
			bool changed = DragDropTargetString(name, pathString, "file");

			value = pathString;

			return changed;
		}
	};
}
