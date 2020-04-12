#include "InspectorFields.hpp"

#include "../Parsers/Json.hpp"

namespace EVA
{
	char* InspectorFields::GetCString(const std::string& value)
	{
		strcpy_s(C_STRING, 10000, value.c_str());
		return C_STRING;
	}

	void InspectorFields::Text(std::string text)
	{
		ImGui::Text(text.c_str(), nullptr);
	}

	void InspectorFields::Text(const char* text)
	{
		ImGui::Text(text, nullptr);
	}

	bool InspectorFields::Int(const char* name, int& value)
	{
		return ImGui::InputInt(name, &value);
	}

	bool InspectorFields::EnterInt(const char* name, int& value)
	{
		auto temp = value;
		if (ImGui::InputInt(name, &temp, 1, 5, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			value = temp;
			return true;
		}

		return false;
	}

	bool InspectorFields::Bool(const char* name, bool& value)
	{
		return ImGui::Checkbox(name, &value);
	}

	bool InspectorFields::Float(const char* name, float& value)
	{
		return ImGui::InputFloat(name, &value, 0.0f, 0.0f, "%.5f");
	}

	bool InspectorFields::Float2(const char* name, glm::vec2& value)
	{
		return ImGui::InputFloat2(name, glm::value_ptr(value));
	}

	bool InspectorFields::Float3(const char* name, glm::vec3& value)
	{
		return ImGui::InputFloat3(name, glm::value_ptr(value));
	}

	bool InspectorFields::Float4(const char* name, glm::vec4& value)
	{
		return ImGui::InputFloat4(name, glm::value_ptr(value));
	}

	bool InspectorFields::String(const char* name, std::string& value)
	{
		auto string = GetCString(value);

		bool changed = ImGui::InputText(name, string, STRING_LENGTH);

		value = string;

		return changed;
	}

	bool InspectorFields::EnterString(const char* name, std::string& value)
	{
		auto string = GetCString(value);
		if (ImGui::InputText(name, string, STRING_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			value = string;
			return true;
		}

		return false;
	}

	bool InspectorFields::DragDropTargetString(const char* name, std::string& value, const char* payloadType)
	{
		auto string = GetCString(value);
		ImGui::BeginGroup();
		if (ImGui::InputText(name, string, STRING_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
		{
			value = string;
			ImGui::EndGroup();
			return true;
		}
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

	bool InspectorFields::ColorPicker(const char* name, glm::vec3& value)
	{
		return ImGui::ColorEdit3(name, glm::value_ptr(value));
	}

	bool InspectorFields::ColorPicker(const char* name, glm::vec4& value)
	{
		return ImGui::ColorEdit4(name, glm::value_ptr(value));
	}

	bool InspectorFields::DragFloat(const char* name, float& value, const float min, const float max, const float step)
	{
		return ImGui::DragFloat(name, &value, step, min, max);
	}

	bool InspectorFields::RangeFloat(const char* name, float& min, float& max, const float step)
	{
		return ImGui::DragFloatRange2(name, &min, &max, step);
	}

	bool InspectorFields::Button(const char* name)
	{
		return ImGui::Button(name);
	}

	bool InspectorFields::Path(const char* name, FS::path& value)
	{
		auto pathString = FileSystem::ToString(value);
		bool changed = InspectorFields::DragDropTargetString(name, pathString, "file");

		value = pathString;

		return changed;
	}

	bool InspectorFields::Serializeable(const char* name, const std::shared_ptr<ISerializeable>& value)
	{
		bool changed = false;
		if (ImGui::CollapsingHeader(name))
		{
			ImGui::Indent(INDENT);

			DataObject data;
			data.mode = DataObject::DataMode::Inspector;
			value->Serialize(data);

			ImGui::Unindent(INDENT);
		}
		return changed;
	}
}