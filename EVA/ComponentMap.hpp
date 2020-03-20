#pragma once

#include <map>
#include <memory>
#include <vector>

#include "ClassMap.hpp"
#include "Component.hpp"

CREATE_CLASS_MAP(EVAComponent, EVA::Component)

// Macro for registering a component. Should be put inside the class declaration
#define REGISTER_COMPONENT(TYPE, NAME) REGISTER_CLASS(EVAComponent, TYPE, NAME)