#pragma once

#include <random>

#include "EVA.hpp"
#include "EVA/Components.hpp"

#include "../Quadtree/Quadtree.hpp"
#include "../Quadtree/QuadtreeMesh.hpp"
#include "WaterMaterial.hpp"


namespace EVA
{
	class Noise : public ISerializeable
	{
		float m_Scale = 50;
		float m_Persistance = 0.6f;
		float m_Lacunarity = 2.0f;

		float m_MaxPossibleHeight;

		int m_Octaves = 6;
		std::vector<glm::vec2> m_OctaveOffsets;

	public:

		const float& scale = m_Scale;
		const float& persistance = m_Persistance;
		const float& lacunarity = m_Lacunarity;

		const float& maxPossibleHeight = m_MaxPossibleHeight;

		const std::vector<glm::vec2>& octaveOffsets = m_OctaveOffsets;

		Noise()
		{
			m_OctaveOffsets = std::vector<glm::vec2>(m_Octaves);
		}

		virtual void Serialize(DataObject& data) override
		{
			if (data.Serialize("scale", m_Scale) ||
				data.Serialize("persistance", m_Persistance) ||
				data.Serialize("lacunarity", m_Lacunarity) ||
				data.Serialize("octaves", m_Octaves))
			{
				std::random_device r;
				std::default_random_engine dre(r());
				std::uniform_real_distribution<float> uniformDist(-100000, 100000);

				float amplitude = 1;
				m_MaxPossibleHeight = 0;

				m_OctaveOffsets.resize(m_Octaves);
				for (int i = 0; i < m_Octaves; i++)
				{
					float offsetX = uniformDist(dre);
					float offsetY = uniformDist(dre);
					m_OctaveOffsets[i] = glm::vec2(offsetX, offsetY);

					m_MaxPossibleHeight += amplitude;
					amplitude *= m_Persistance;
				}
			}

			if (data.mode == DataObject::DataMode::Inspector)
			{
				InspectorFields::Text("Max height: " + std::to_string(m_MaxPossibleHeight));

				for (int i = 0; i < m_Octaves; i++)
				{
					InspectorFields::Text("Octave offset " + std::to_string(i) + ": (" + std::to_string(m_OctaveOffsets[i].x) + ", " + std::to_string(m_OctaveOffsets[i].y) + ")");
				}
			}
		}
	};

	class Water : public Component, public IUpdateComponent, public IRenderComponent
	{
		REGISTER_COMPONENT(Water, "EVA::Water");

		std::unique_ptr<Quadtree> m_Quadtree;

		std::shared_ptr<WaterMaterial> m_Material = nullptr;
		FS::path m_ShaderPath;
		std::shared_ptr<QuadtreeMesh> m_Mesh = nullptr;
		std::vector<QuadtreeMeshData> m_MeshData;

		std::string m_TargetName;
		Transform* m_Target;

		std::shared_ptr<Noise> m_Noise;

		glm::vec2 m_WaveSpeed;

	public:

		const std::shared_ptr<Noise>& noise = m_Noise;
		const glm::vec2& waveSpeed = m_WaveSpeed;

		void Start() override;
		void Update(float deltaTime) override;

		void Render() override;

		virtual void Serialize(DataObject& data) override;

		static std::vector<glm::vec2> GeneratePatch();
	};

}
