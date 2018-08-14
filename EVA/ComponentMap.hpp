#pragma once

#include <map>
#include <memory>
#include <vector>

#include "ClassMap.hpp"
#include "Component.hpp"

namespace EVA
{
	// Macro for registering a component. Should be put inside the class declaration
	#define REGISTER_COMPONENT_HPP(TYPE) \
    REGISTER_CLASS_HPP(Component, TYPE)

	// Macro for registering a component
	#define REGISTER_COMPONENT_CPP(TYPE, NAME) \
    REGISTER_CLASS_CPP(Component, TYPE, NAME)

	/**
	 * \brief Keeps track of all registerd components
	 */
	class ComponentMap : public ClassMap<Component>
	{

	};
}