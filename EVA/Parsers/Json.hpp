#pragma once

#define RAPIDJSON_HAS_STDSTRING 1

#include <cstdio>
#include <memory>

#include "glm/glm.hpp"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/filewritestream.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"

#include "../FileSystem.hpp"
#include "../Editor/InspectorFields.hpp"

namespace EVA
{

	class Json
	{

	public:

		typedef rapidjson::Value Value;
		typedef rapidjson::Document Document;
		typedef rapidjson::GenericValue<rapidjson::UTF8<>> Generic;
		typedef rapidjson::Document::AllocatorType Allocator;
		typedef rapidjson::GenericStringRef<char> StringRef;

		/**
		 * \brief Opens a JSON documnet form the current path
		 * \param path The path to the document
		 * \return A pointer to the parsed documnet, or nullptr if no document is found
		 */
		static std::shared_ptr<Document> Open(const FS::path& path)
		{
			const auto fp = fopen(FileSystem::ToString(path).c_str(), "rb");

			if (fp == nullptr)
				return nullptr;

			char readBuffer[65536];

			rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));

			auto d = std::make_shared<Document>();
			d->ParseStream(is);

			fclose(fp);
			return d;
		}

		static bool Save(const Document* d, const FS::path& path)
		{
			const auto fp = fopen(FileSystem::ToString(path).c_str(), "wb");
			char writeBuffer[65536];
			rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
			rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
			const auto r = d->Accept(writer);

			fclose(fp);
			return r;
		}

		static bool IsVec4(Value& jsonValue)
		{
			if (jsonValue.IsArray())
			{
				const auto a = jsonValue.GetArray();
				if (a.Size() == 4 && a[0].IsDouble() && a[1].IsDouble() && a[2].IsDouble() && a[3].IsDouble())
					return true;
			}

			return false;
		}

		static glm::vec4 GetVec4(Value& jsonValue)
		{
			const auto a = jsonValue.GetArray();
			return { a[0].GetDouble() , a[1].GetDouble() , a[2].GetDouble() , a[3].GetDouble() };
		}

		static float GetFloat(const Value& json, const char* key, const float defaultValue)
		{
			if (json.HasMember(key) && json[key].IsNumber())
				return (float)json[key].GetDouble();

			return defaultValue;
		}
	};

	class DataObject
	{
		Json::Generic& m_Json;
		Json::Allocator* m_Allocator;

	public:

		enum DataMode
		{
			Save,
			Load,
			Inspector
		};

		DataMode mode = Save;
		bool changed = false;

		Json::Generic& json = m_Json;
		Json::Allocator* allocator = m_Allocator;

		DataObject() : m_Json(Json::Generic()), m_Allocator(nullptr), mode(Inspector)
		{}

		explicit DataObject(Json::Generic& json) : m_Json(json), m_Allocator(nullptr), mode(Load)
		{}

		explicit DataObject(Json::Generic& json, Json::Allocator* allocator) : m_Json(json), m_Allocator(allocator), mode(Save)
		{}

		int GetInt(const char* key, const int defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsInt())
				return m_Json[key].GetInt();

			return defaultValue;
		}

		void SetInt(const Json::StringRef& key, const int value) const
		{
			m_Json.AddMember(key, value, *m_Allocator);
		}

		bool GetBool(const char* key, const bool defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsBool())
				return m_Json[key].GetBool();

			return defaultValue;
		}

		void SetBool(const Json::StringRef& key, const bool value) const
		{
			m_Json.AddMember(key, value, *m_Allocator);
		}

		float GetFloat(const char* key, const float defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsNumber())
				return (float)m_Json[key].GetDouble();

			return defaultValue;
		}

		void SetFloat(const Json::StringRef& key, const float value) const
		{
			m_Json.AddMember(key, value, *m_Allocator);
		}

		glm::vec2 GetVec2(const char* key, const glm::vec2 defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsArray())
			{
				const auto a = m_Json[key].GetArray();
				if (a.Size() == 2 && a[0].IsDouble() && a[1].IsDouble())
					return { a[0].GetDouble(), a[1].GetDouble() };
			}

			return defaultValue;
		}

		void SetVec2(const Json::StringRef& key, const glm::vec2 value) const
		{
			Json::Value v;
			v.SetArray();
			v.PushBack(value.x, *m_Allocator);
			v.PushBack(value.y, *m_Allocator);

			m_Json.AddMember(key, v, *m_Allocator);
		}

		glm::vec3 GetVec3(const char* key, const glm::vec3 defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsArray())
			{
				const auto a = m_Json[key].GetArray();
				if (a.Size() == 3 && a[0].IsDouble() && a[1].IsDouble() && a[2].IsDouble())
					return { a[0].GetDouble(), a[1].GetDouble(), a[2].GetDouble() };
			}

			return defaultValue;
		}

		void SetVec3(const Json::StringRef& key, const glm::vec3 value) const
		{
			Json::Value v;
			v.SetArray();
			v.PushBack(value.x, *m_Allocator);
			v.PushBack(value.y, *m_Allocator);
			v.PushBack(value.z, *m_Allocator);

			m_Json.AddMember(key, v, *m_Allocator);
		}

		glm::vec4 GetVec4(const char* key, const glm::vec4 defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsArray())
			{
				const auto a = m_Json[key].GetArray();
				if (a.Size() == 4 && a[0].IsDouble() && a[1].IsDouble() && a[2].IsDouble() && a[3].IsDouble())
					return { a[0].GetDouble(), a[1].GetDouble(), a[2].GetDouble(), a[3].GetDouble() };
			}

			return defaultValue;
		}

		void SetVec4(const Json::StringRef& key, const glm::vec4 value) const
		{
			Json::Value v;
			v.SetArray();
			v.PushBack(value.x, *m_Allocator);
			v.PushBack(value.y, *m_Allocator);
			v.PushBack(value.z, *m_Allocator);
			v.PushBack(value.w, *m_Allocator);

			m_Json.AddMember(key, v, *m_Allocator);
		}

		std::string GetString(const char* key, const std::string& defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsString())
				return m_Json[key].GetString();

			return defaultValue;
		}

		void SetString(const Json::StringRef& key, const std::string& value) const
		{
			m_Json.AddMember(key, value, *m_Allocator);
		}

		FS::path GetPath(const char* key, const FS::path& defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsString())
				return FS::path(m_Json[key].GetString());

			return defaultValue;
		}

		void SetPath(const Json::StringRef& key, const FS::path& value) const
		{
			m_Json.AddMember(key, FileSystem::ToString(value), *m_Allocator);
		}


		bool Serialize(const Json::StringRef& key, int& value)
		{
			switch (mode)
			{
			case Save: 
				SetInt(key, value); 
				break;

			case Load: 
				value = GetInt(key, value); 
				changed = true;
				return true;

			case Inspector: 
				bool c = InspectorFields::Int(key, value);
				if (c) changed = true;
				return c;
			}

			return false;
		}

		bool Serialize(const Json::StringRef& key, float& value)
		{
			switch (mode)
			{
			case Save: 
				SetFloat(key, value); 
				break;

			case Load: 
				value = GetFloat(key, value); 
				changed = true;
				return true;

			case Inspector:
				bool c = InspectorFields::Float(key, value);
				if (c) changed = true;
				return c;
			}

			return false;
		}

		bool Serialize(const Json::StringRef& key, bool& value)
		{
			switch (mode)
			{
			case Save: 
				SetBool(key, value); 
				break;

			case Load: 
				value = GetBool(key, value); 
				changed = true;
				return true;

			case Inspector:
				bool c = InspectorFields::Bool(key, value);
				if (c) changed = true;
				return c;
			}

			return false;
		}

		bool Serialize(const Json::StringRef& key, std::string& value)
		{
			switch (mode)
			{
			case Save: 
				SetString(key, value); 
				break;

			case Load: 
				value = GetString(key, value); 
				changed = true;
				return true;

			case Inspector:
				bool c = InspectorFields::EnterString(key, value);
				if (c) changed = true;
				return c;
			}

			return false;
		}

		bool Serialize(const Json::StringRef& key, glm::vec2& value)
		{
			switch (mode)
			{
			case Save:
				SetVec2(key, value); 
				break;

			case Load: 
				value = GetVec2(key, value); 
				changed = true;
				return true;

			case Inspector:
				bool c = InspectorFields::Float2(key, value);
				if (c) changed = true;
				return c;
			}

			return false;
		}

		bool Serialize(const Json::StringRef& key, glm::vec3& value)
		{
			switch (mode)
			{
			case Save: 
				SetVec3(key, value); 
				break;

			case Load: 
				value = GetVec3(key, value); 
				changed = true;
				return true;

			case Inspector:
				bool c = InspectorFields::Float3(key, value);
				if (c) changed = true;
				return c;
			}

			return false;
		}

		bool Serialize(const Json::StringRef& key, glm::vec4& value)
		{
			switch (mode)
			{
			case Save:
				SetVec4(key, value);
				break;

			case Load:
				value = GetVec4(key, value);
				changed = true;
				return true;

			case Inspector:
				bool c = InspectorFields::Float4(key, value);
				if (c) changed = true;
				return c;
			}

			return false;
		}

		bool Serialize(const Json::StringRef& key, glm::quat& value)
		{
			glm::vec4 v;

			switch (mode)
			{
			case Save:
				SetVec4(key, { value.x, value.y, value.z, value.w });
				break;

			case Load:
				v = GetVec4(key, { value.x, value.y, value.z, value.w });
				value = glm::quat(v.w, v.x, v.y, v.z);
				changed = true;
				return true;

			case Inspector:
				v = glm::vec4(value.x, value.y, value.z, value.w);
				bool c = InspectorFields::Float4(key, v);
				value = glm::quat(v.w, v.x, v.y, v.z);

				if (c) changed = true;
				return c;
			}

			return false;
		}

		bool Serialize(const Json::StringRef& key, FS::path& value)
		{
			switch (mode)
			{
			case Save: 
				SetPath(key, value); 
				break;

			case Load: 
				value = GetPath(key, value); 
				changed = true; 
				return true;

			case Inspector:
				bool c = InspectorFields::Path(key, value);
				if (c) changed = true;
				return c;
			}

			return false;
		}

	};

	class ISerializeable
	{
	public:
		virtual void Serialize(DataObject& data)
		{

		}
	};

}