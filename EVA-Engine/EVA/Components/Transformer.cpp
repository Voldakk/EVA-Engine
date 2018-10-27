#include "Transformer.hpp"

namespace EVA
{
	REGISTER_COMPONENT_CPP(Transformer, "EVA::Transformer")

	void Transformer::Update(const float deltaTime)
	{
		transform->Translate(m_Translation * deltaTime);
		transform->SetScale(transform->localScale + m_Scaling * deltaTime);
		transform->Rotate(m_Rotation * deltaTime);
	}

	void Transformer::Serialize(DataObject& data)
	{
		data.Serialize("translation", m_Translation);
		data.Serialize("scaling", m_Scaling);
		data.Serialize("rotation", m_Rotation);
	}
}
