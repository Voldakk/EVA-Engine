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


		template <typename T>
		T Get(const char* key, const T defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].Is<T>())
				return m_Json[key].Get<T>();

			return defaultValue;
		}

		template <typename T>
		void Set(const Json::StringRef& key, const T value) const
		{
			m_Json.AddMember(key, value, *m_Allocator);
		}

		template <>
		glm::vec2 Get<glm::vec2>(const char* key, const glm::vec2 defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsArray())
			{
				const auto a = m_Json[key].GetArray();
				if (a.Size() == 2 && a[0].IsDouble() && a[1].IsDouble())
					return { a[0].GetDouble(), a[1].GetDouble() };
			}

			return defaultValue;
		}

		template <>
		void Set<glm::vec2>(const Json::StringRef& key, const glm::vec2 value) const
		{
			Json::Value v;
			v.SetArray();
			v.PushBack(value.x, *m_Allocator);
			v.PushBack(value.y, *m_Allocator);

			m_Json.AddMember(key, v, *m_Allocator);
		}

		template <>
		glm::vec3 Get<glm::vec3>(const char* key, const glm::vec3 defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsArray())
			{
				const auto a = m_Json[key].GetArray();
				if (a.Size() == 3 && a[0].IsDouble() && a[1].IsDouble() && a[2].IsDouble())
					return { a[0].GetDouble(), a[1].GetDouble(), a[2].GetDouble() };
			}

			return defaultValue;
		}

		template <>
		void Set<glm::vec3>(const Json::StringRef& key, const glm::vec3 value) const
		{
			Json::Value v;
			v.SetArray();
			v.PushBack(value.x, *m_Allocator);
			v.PushBack(value.y, *m_Allocator);
			v.PushBack(value.z, *m_Allocator);

			m_Json.AddMember(key, v, *m_Allocator);
		}


		template <>
		glm::vec4 Get<glm::vec4>(const char* key, const glm::vec4 defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsArray())
			{
				const auto a = m_Json[key].GetArray();
				if (a.Size() == 4 && a[0].IsDouble() && a[1].IsDouble() && a[2].IsDouble() && a[3].IsDouble())
					return { a[0].GetDouble(), a[1].GetDouble(), a[2].GetDouble(), a[3].GetDouble() };
			}

			return defaultValue;
		}

		template <>
		void Set<glm::vec4>(const Json::StringRef& key, const glm::vec4 value) const
		{
			Json::Value v;
			v.SetArray();
			v.PushBack(value.x, *m_Allocator);
			v.PushBack(value.y, *m_Allocator);
			v.PushBack(value.z, *m_Allocator);
			v.PushBack(value.w, *m_Allocator);

			m_Json.AddMember(key, v, *m_Allocator);
		}


		template <>
		glm::quat Get<glm::quat>(const char* key, const glm::quat defaultValue) const
		{
			auto vec4 = Get<glm::vec4>(key, glm::vec4(defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w));
			return glm::quat(vec4.w, vec4.x, vec4.y, vec4.z);
		}

		template <>
		void Set<glm::quat>(const Json::StringRef& key, const glm::quat value) const
		{
			Set<glm::vec4>(key, glm::vec4(value.x, value.y, value.z, value.w));
		}


		template <>
		FS::path Get<FS::path>(const char* key, const FS::path defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsString())
				return FS::path(m_Json[key].GetString());

			return defaultValue;
		}

		template <>
		void Set<FS::path>(const Json::StringRef& key, const FS::path value) const
		{
			m_Json.AddMember(key, FileSystem::ToString(value), *m_Allocator);
		}

		template <>
		std::vector<int> Get<std::vector<int>>(const char* key, const std::vector<int> defaultValue) const
		{
			if (m_Json.HasMember(key) && m_Json[key].IsArray())
			{
				const auto a = m_Json[key].GetArray();
				auto arr = std::vector<int>();
				for (size_t i = 0; i < a.Size(); i++)
				{
					arr.push_back(a[i].GetInt());
				}
				return arr;
			}

			return defaultValue;
		}

		template <>
		void Set<std::vector<int>>(const Json::StringRef& key, const std::vector<int> value) const
		{
			Json::Value v;
			v.SetArray();
			for (size_t i = 0; i < value.size(); i++)
			{
				v.PushBack(value[i], *m_Allocator);
			}
			m_Json.AddMember(key, v, *m_Allocator);
		}

		template <typename T>
		bool Serialize(const Json::StringRef& key, T& value)
		{
			switch (mode)
			{
			case Save:
				Set<T>(key, value);
				break;

			case Load:
				value = Get<T>(key, value);
				changed = true;
				return true;

			case Inspector:
				bool c = InspectorFields::Default(key, value);
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