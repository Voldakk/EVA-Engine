#pragma once

#include <map>
#include <memory>
#include <vector>

#include "ClassMap.hpp"
#include "Material.hpp"

namespace EVA
{
	// Macro for registering a component. Should be put inside the class declaration
	#define REGISTER_MATERIAL_HPP(TYPE) \
    REGISTER_CLASS_HPP(Material, TYPE)

	// Macro for registering a component
	#define REGISTER_MATERIAL_CPP(TYPE, NAME) \
    REGISTER_CLASS_CPP(Material, TYPE, NAME)

	/**
	 * \brief Keeps track of all registerd components
	 */
	class MaterialMap : public ClassMap<Material>
	{

	};
}