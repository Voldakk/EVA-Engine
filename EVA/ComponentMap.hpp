#pragma once

#include <map>
#include <memory>
#include <vector>

#include "ClassMap.hpp"
#include "Component.hpp"

namespace EVA
{
	// Macro for registering a component. Should be put inside the class declaration
	#define REGISTER_COMPONENT(TYPE, NAME) \
    REGISTER_CLASS(EVA::Component, TYPE, NAME)

	/**
	 * \brief Keeps track of all registerd components
	 */
	class ComponentMap : public ClassMap<Component>
	{

	};
}