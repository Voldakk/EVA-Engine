#pragma once

#include "ClassMap.hpp"
#include "Material.hpp"

namespace EVA
{
	// Macro for registering a material. Should be put inside the class declaration
#define REGISTER_MATERIAL_HPP(TYPE) \
    REGISTER_CLASS_HPP(EVA::Material, TYPE)

	// Macro for registering a material
#define REGISTER_MATERIAL_CPP(TYPE, NAME) \
    REGISTER_CLASS_CPP(EVA::Material, TYPE, NAME)

	/**
	 * \brief Keeps track of all registerd materials
	 */
	class EVA_API MaterialMap : public ClassMap<Material>
	{
	};
}
