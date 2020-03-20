#pragma once

#include <map>
#include <memory>
#include <vector>

#include "ClassMap.hpp"
#include "Material.hpp"

CREATE_CLASS_MAP(EVAMaterial, EVA::Material)

// Macro for registering a material. Should be put inside the class declaration
#define REGISTER_MATERIAL(TYPE, NAME) REGISTER_CLASS(EVAMaterial, TYPE, NAME)