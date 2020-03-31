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

#include "ISerializeable.hpp"
#include "../Asset.hpp"

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
			FILE* fp;
			const errno_t error = fopen_s(&fp, FileSystem::ToString(path).c_str(), "rb");

			if (error != 0)
				return nullptr;

			char* readBuffer = new char[65536];
			rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
			auto d = std::make_shared<Document>();
			d->ParseStream(is);

			fclose(fp);
			delete[] readBuffer;
			return d;
		}

		static bool Save(const Document* d, const FS::path& path)
		{
			FILE* fp;
			const errno_t error = fopen_s(&fp, FileSystem::ToString(path).c_str(), "wb");

			if (error != 0)
				return false;

			char* writeBuffer = new char[65536];
			rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
			rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
			const auto r = d->Accept(writer);

			fclose(fp);
			delete[] writeBuffer;
			return r;
		}
	};

	class DataObject
	{
		Json::Generic m_JsonInternal;
		Json::Generic& m_Json;
		Json::Allocator* m_Allocator;

	public:

		enum class DataMode
		{
			Save,
			Load,
			Inspector
		};

		DataMode mode = DataMode::Save;
		bool changed = false;

		Json::Generic& json = m_Json;
		Json::Allocator* allocator = m_Allocator;

		DataObject() : m_JsonInternal(Json::Generic()), m_Json(m_JsonInternal), m_Allocator(nullptr), mode(DataMode::Inspector)
		{}

		explicit DataObject(Json::Generic& json) : m_Json(json), m_Allocator(nullptr), mode(DataMode::Load)
		{}

		explicit DataObject(Json::Generic& json, Json::Allocator* allocator) : m_Json(json), m_Allocator(allocator), mode(DataMode::Save)
		{}

		template <typename T>
		bool Serialize(const Json::StringRef& key, T& value);

		template <typename T>
		T Get(const char* key, const T defaultValue) const;

		template <typename T>
		void Set(const Json::StringRef& key, const T value) const;


		template <>
		glm::vec2 Get(const char* key, const glm::vec2 defaultValue) const;

		template <>
		void Set(const Json::StringRef& key, const glm::vec2 value) const;


		template <>
		glm::vec3 Get(const char* key, const glm::vec3 defaultValue) const;

		template <>
		void Set(const Json::StringRef& key, const glm::vec3 value) const;


		template <>
		glm::vec4 Get(const char* key, const glm::vec4 defaultValue) const;

		template <>
		void Set(const Json::StringRef& key, const glm::vec4 value) const;


		template <>
		glm::quat Get(const char* key, const glm::quat defaultValue) const;

		template <>
		void Set(const Json::StringRef& key, const glm::quat value) const;


		template <>
		FS::path Get(const char* key, const FS::path defaultValue) const;

		template <>
		void Set(const Json::StringRef& key, const FS::path value) const;


		template <typename T, typename Alloc>
		std::vector<T, Alloc> Get(const char* key, const std::vector<T, Alloc> defaultValue) const;

		template <typename T, typename Alloc>
		void Set(const Json::StringRef& key, const std::vector<T, Alloc> value) const;


		template <typename T = typename ISerializeable>
		std::shared_ptr<T> DataObject::Get(const char* key, const std::shared_ptr<T> defaultValue) const;

		template <typename T = typename ISerializeable>
		void Set(const Json::StringRef& key, const std::shared_ptr<T> value) const;


		/*template <typename T = typename Asset>
		std::shared_ptr<T> DataObject::Get(const char* key, const std::shared_ptr<T> defaultValue) const;

		template <typename T = typename Asset>
		void Set(const Json::StringRef& key, const std::shared_ptr<T> value) const;*/
	};

	template<typename T>
	inline bool DataObject::Serialize(const Json::StringRef& key, T& value)
	{
		switch (mode)
		{
		case DataMode::Save:
			Set(key, value);
			break;

		case DataMode::Load:
			value = Get(key, value);
			changed = true;
			return true;

		case DataMode::Inspector:
			bool c = InspectorFields::Default(key, value);
			if (c) changed = true;
			return c;
		}

		return false;
	}

	template<typename T>
	inline T DataObject::Get(const char* key, const T defaultValue) const
	{
		if (m_Json.HasMember(key) && m_Json[key].Is<T>())
			return m_Json[key].Get<T>();

		return defaultValue;
	}

	template<typename T>
	inline void DataObject::Set(const Json::StringRef& key, const T value) const
	{
		m_Json.AddMember(key, value, *m_Allocator);
	}

	template<>
	inline glm::vec2 DataObject::Get(const char* key, const glm::vec2 defaultValue) const
	{
		if (m_Json.HasMember(key) && m_Json[key].IsArray())
		{
			const auto a = m_Json[key].GetArray();
			if (a.Size() == 2 && a[0].IsDouble() && a[1].IsDouble())
				return { a[0].GetDouble(), a[1].GetDouble() };
		}

		return defaultValue;
	}

	template<>
	inline void DataObject::Set(const Json::StringRef& key, const glm::vec2 value) const
	{
		Json::Value v;
		v.SetArray();
		v.PushBack(value.x, *m_Allocator);
		v.PushBack(value.y, *m_Allocator);

		m_Json.AddMember(key, v, *m_Allocator);
	}

	template<>
	inline glm::vec3 DataObject::Get(const char* key, const glm::vec3 defaultValue) const
	{
		if (m_Json.HasMember(key) && m_Json[key].IsArray())
		{
			const auto a = m_Json[key].GetArray();
			if (a.Size() == 3 && a[0].IsDouble() && a[1].IsDouble() && a[2].IsDouble())
				return { a[0].GetDouble(), a[1].GetDouble(), a[2].GetDouble() };
		}

		return defaultValue;
	}

	template<>
	inline void DataObject::Set(const Json::StringRef& key, const glm::vec3 value) const
	{
		Json::Value v;
		v.SetArray();
		v.PushBack(value.x, *m_Allocator);
		v.PushBack(value.y, *m_Allocator);
		v.PushBack(value.z, *m_Allocator);

		m_Json.AddMember(key, v, *m_Allocator);
	}

	template<>
	inline glm::vec4 DataObject::Get(const char* key, const glm::vec4 defaultValue) const
	{
		if (m_Json.HasMember(key) && m_Json[key].IsArray())
		{
			const auto a = m_Json[key].GetArray();
			if (a.Size() == 4 && a[0].IsDouble() && a[1].IsDouble() && a[2].IsDouble() && a[3].IsDouble())
				return { a[0].GetDouble(), a[1].GetDouble(), a[2].GetDouble(), a[3].GetDouble() };
		}

		return defaultValue;
	}

	template<>
	inline void DataObject::Set(const Json::StringRef& key, const glm::vec4 value) const
	{
		Json::Value v;
		v.SetArray();
		v.PushBack(value.x, *m_Allocator);
		v.PushBack(value.y, *m_Allocator);
		v.PushBack(value.z, *m_Allocator);
		v.PushBack(value.w, *m_Allocator);

		m_Json.AddMember(key, v, *m_Allocator);
	}

	template<>
	inline glm::quat DataObject::Get(const char* key, const glm::quat defaultValue) const
	{
		auto vec4 = Get<glm::vec4>(key, glm::vec4(defaultValue.x, defaultValue.y, defaultValue.z, defaultValue.w));
		return glm::quat(vec4.w, vec4.x, vec4.y, vec4.z);
	}

	template<>
	inline void DataObject::Set(const Json::StringRef& key, const glm::quat value) const
	{
		Set<glm::vec4>(key, glm::vec4(value.x, value.y, value.z, value.w));
	}

	template<>
	inline FS::path DataObject::Get(const char* key, const FS::path defaultValue) const
	{
		if (m_Json.HasMember(key) && m_Json[key].IsString())
			return FS::path(m_Json[key].GetString());

		return defaultValue;
	}

	template<>
	inline void DataObject::Set(const Json::StringRef& key, const FS::path value) const
	{
		m_Json.AddMember(key, FileSystem::ToString(value), *m_Allocator);
	}

	template<typename T, typename Alloc>
	inline std::vector<T, Alloc> DataObject::Get(const char* key, const std::vector<T, Alloc> defaultValue) const
	{
		if (m_Json.HasMember(key) && m_Json[key].IsArray())
		{
			const auto a = m_Json[key].GetArray();
			auto arr = std::vector<T, Alloc>();
			for (size_t i = 0; i < a.Size(); i++)
			{
				arr.push_back(a[i].Get<T>());
			}
			return arr;
		}

		return defaultValue;
	}

	template<typename T, typename Alloc>
	inline void DataObject::Set(const Json::StringRef& key, const std::vector<T, Alloc> value) const
	{
		Json::Value v;
		v.SetArray();
		for (size_t i = 0; i < value.size(); i++)
		{
			v.PushBack(value[i], *m_Allocator);
		}
		m_Json.AddMember(key, v, *m_Allocator);
	}

	template<class T>
	inline std::shared_ptr<T> DataObject::Get(const char* key, const std::shared_ptr<T> defaultValue) const
	{
		if (m_Json.HasMember(key) && m_Json[key].IsObject())
		{
			auto o = std::make_shared<T>();
			auto data = DataObject(m_Json[key]);
			data.mode = DataMode::Load;
			o->Serialize(data);
			return o;
		}

		return defaultValue;
	}

	template<class T>
	inline void DataObject::Set(const Json::StringRef& key, const std::shared_ptr<T> value) const
	{
		if (value == nullptr)
			return;

		Json::Value v;
		v.SetObject();

		DataObject data(v, m_Allocator);
		data.mode = DataMode::Save;

		value->Serialize(data);

		m_Json.AddMember(key, v, *m_Allocator);
	}
}
