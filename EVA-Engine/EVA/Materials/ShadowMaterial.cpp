#include "ShadowMaterial.hpp"

#include "../Scene.hpp"
#include "../Application.hpp"

namespace EVA
{
	REGISTER_MATERIAL_CPP(ShadowMaterial, "EVA::ShadowMaterial")

	void ShadowMaterial::SetMaterialUniforms(Scene* scene) const
	{
	}
}
