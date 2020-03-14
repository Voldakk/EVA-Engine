#pragma once

#include <memory>

#include "../Mesh.hpp"	

#include "../Material.hpp"	
#include "../Component.hpp"
#include "../ComponentMap.hpp"

namespace EVA
{

	class MeshRenderer : public Component
	{
		REGISTER_COMPONENT(MeshRenderer, "EVA::MeshRenderer")

		std::shared_ptr<Mesh> m_Mesh;
		std::shared_ptr<Material> m_Material;

		FS::path m_ModelPath;
		unsigned int m_MeshIndex = 0;

	public:

		// Public read only fields
		const std::shared_ptr<Mesh>& mesh = m_Mesh;
		const std::shared_ptr<Material>& material = m_Material;

		~MeshRenderer() override;

		void Start() override;

		void Set(const std::shared_ptr<Mesh>& mesh, const std::shared_ptr<Material>& material);

		void Render() const;

		virtual void Serialize(DataObject& data) override;

	private:

		std::shared_ptr<Mesh> GetMesh(const FS::path& modelPath, unsigned int meshIndex);
		
	};
}
