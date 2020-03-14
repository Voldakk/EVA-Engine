#pragma once

#include <map>
#include <memory>
#include <vector>

namespace EVA
{
	// Macro for registering a class. Should be put inside the class declaration
#define REGISTER_CLASS(BASETYPE, TYPE, NAME) \
    static inline EVA::ClassRegister<BASETYPE, TYPE> m_Register = NAME;\
	public:\
	std::string GetTypeId() const\
	{\
		return m_Register.typeId;\
	}\
	private:

	/**
	* \brief Keeps track of all registerd classes
	*/
	template <class ClassType>
	class ClassMap
	{
	public:

		typedef std::map<std::string, std::shared_ptr<ClassType>(*)()> map_type;
		static map_type * map;

		static map_type * GetMap()
		{
			if (!map)
			{
				map = new map_type();
			}
			return map;
		}

		static std::shared_ptr<ClassType> Create(const std::string& id)
		{
			const auto it = GetMap()->find(id);

			if (it == GetMap()->end())
				return nullptr;

			return it->second();
		}

		template<typename T>
		static std::shared_ptr<ClassType> CreateT()
		{
			return std::make_shared<T>();
		}

		static std::vector<std::string> GetIds()
		{
			std::vector<std::string> ids;

			for (map_type::const_iterator it = map->begin(); it != map->end(); ++it)
			{
				ids.push_back(it->first);
			}

			return ids;
		}
	};

	template<typename BaseT, typename T>
	struct ClassRegister
	{
		std::string typeId;
		ClassRegister(std::string const& s)
		{
			typeId = s;
			ClassMap<BaseT>::GetMap()->insert(std::make_pair(s, &ClassMap<BaseT>::CreateT<T>));
		}
	};
}
