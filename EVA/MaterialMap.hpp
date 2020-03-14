#pragma once

#include <map>
#include <memory>
#include <vector>

#include "ClassMap.hpp"
#include "Material.hpp"

namespace EVA
{
	// Macro for registering a component. Should be put inside the class declaration
	#define REGISTER_MATERIAL(TYPE, NAME) \
    REGISTER_CLASS(Material, TYPE, NAME)

	/**
	 * \brief Keeps track of all registerd components
	 */
	class MaterialMap : public ClassMap<Material>
	{

	};
}