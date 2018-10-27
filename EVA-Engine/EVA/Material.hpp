#pragma once

#include <memory>

#include "Asset.hpp"
#include "Shader.hpp"

#include "EVA/OpenGL.hpp"
#include "FileSystem.hpp"
#include "TextureManager.hpp"
#include "Mesh.hpp"

#include "Parsers/Json.hpp"

namespace EVA
{
	class Scene;

	class Transform;

	struct InstancedMeshData
	{
		unsigned int instanceCount = 0;
		std::unique_ptr<VertexBuffer> matrixBuffer;
		bool dirty = true;
	};

	/**
	 * \brief A material holds a Shader, textures and other material properties
	 */
	class Material : public Asset
	{
	protected:

		static Material* m_ActiveMaterial;

		// Instanced
		bool m_UseInstancing = false;
		std::map<std::shared_ptr<Mesh>, InstancedMeshData> m_MatrixBuffers;

	public:

		const bool& useInstancing = m_UseInstancing;
		void SetMbo(const std::shared_ptr<Mesh>& mesh, const std::vector<glm::mat4>& models);
		bool HasMbo(const std::shared_ptr<Mesh>& mesh) const;
		InstancedMeshData* GetMbo(const std::shared_ptr<Mesh>& mesh);
		void SetDirtyAll();
		void SetDirty(const std::shared_ptr<Mesh>& mesh, bool value = true);
		bool IsDirty(const std::shared_ptr<Mesh>& mesh);

		void SetUseInstancing(bool value);
		// End instanced

		FS::path path;
		std::shared_ptr<Shader> shader;

		bool cullFront = false;
		bool cullBack = true;

		bool castShadows = true;

		Material() = default;

		static void SetNoActive();

		void Activate(Scene* scene, Transform* transform);

		virtual void SetMaterialUniforms(Scene* scene) const;

		virtual void SetObjectUniforms(Transform* transform) const;

		void SaveToFile();

		void Serialize(DataObject& data) override;
	};
}
