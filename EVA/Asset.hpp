#pragma once

#include <string>

#include "FileSystem.hpp"
#include "Parsers/ISerializeable.hpp"

namespace EVA
{
	/**
	* \brief Base class for assets
	*/
	class Asset : public ISerializeable
	{

	public:

		FS::path path;

		/**
		* \brief Default constructor
		*/
		Asset() = default;

		/**
		* \brief Default destructor
		*/
		virtual ~Asset() = default;

		/**
		* \brief Saves values to the given DataObject
		* \param data The DataObject
		*/
		//virtual void SaveAsset(DataObject& data) const;

		/**
		* \brief Loads values from the given DataObject
		* \param data The DataObject
		*/
		//virtual void LoadAsset(const DataObject data);

		/**
		* \brief Draws the asset inspector
		*/
		//virtual void DrawInspector();

		/**
		* \brief 
		*/
		virtual std::string GetTypeId() const;
	};
}
