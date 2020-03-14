#include "HideOnStart.hpp"

namespace EVA
{
	void HideOnStart::Update(float deltaTime)
	{
		const auto mrs = gameObject->GetComponentsOfType<MeshRenderer>();

		for (auto& mr : mrs)
		{
			mr->SetActive(false);
		}
	}
}
