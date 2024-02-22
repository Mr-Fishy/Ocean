
#include "ocpch.hpp"
#include "SceneSerializer.hpp"

#include "Ocean/Scene/Entity.hpp"
#include "Ocean/Scene/Components.hpp"
#include "Ocean/Utils/InternalUtils.hpp"

// libs
#include <yaml-cpp/yaml.h>

// std
#include <fstream>

namespace YAML {

	template<>
	struct convert<glm::vec3> {
		static Node encode(const glm::vec3& rhs) {
			Node node;

			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);

			return node;
		}

		static bool decode(const Node& node, glm::vec3& rhs) {
			if (!node.IsSequence() || node.size() != 3)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();

			return true;
		}
	};

	template<>
	struct convert<glm::vec4> {
		static Node encode(const glm::vec4& rhs) {
			Node node;

			node.push_back(rhs.x);
			node.push_back(rhs.y);
			node.push_back(rhs.z);
			node.push_back(rhs.w);

			return node;
		}

		static bool decode(const Node& node, glm::vec4& rhs) {
			if (!node.IsSequence() || node.size() != 4)
				return false;

			rhs.x = node[0].as<float>();
			rhs.y = node[1].as<float>();
			rhs.z = node[2].as<float>();
			rhs.w = node[3].as<float>();

			return true;
		}
	};

}	// YAML

namespace Ocean {

	static YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec3& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << YAML::EndSeq;

		return out;
	}

	static YAML::Emitter& operator << (YAML::Emitter& out, const glm::vec4& v) {
		out << YAML::Flow;
		out << YAML::BeginSeq << v.x << v.y << v.z << v.w << YAML::EndSeq;

		return out;
	}

	SceneSerializer::SceneSerializer(const Ref<Scene>& scene) : m_Scene(scene) { }

	template<typename T, typename Function>
	static void SerializeComponent(Entity entity, YAML::Emitter& out, Function function) {
		using namespace YAML;

		if (entity.HasComponent<T>()) {
			out << Key << GetTypeName<T>(); // T Component
			out << BeginMap; // Component
				function(entity.GetComponent<T>()); // Component Specific Serialization
			out << EndMap; // Component
		}
	}

	static void SerializeEntity(YAML::Emitter& out, Entity entity) {
		using namespace YAML;

		out << BeginMap; // Entity
			out << Key << "Entity" << Value << "12837192831273"; // TODO: Entity GUID goes here

			SerializeComponent<TagComponent>(entity, out, [&](auto& component) {
				out << Key << "Tag" << Value << component.Tag;
			});

			SerializeComponent<TransformComponent>(entity, out, [&](auto& component) {
				out << Key << "Translation" << Value << component.Translation;
				out << Key << "Rotation" << Value << component.Rotation;
				out << Key << "Scale" << Value << component.Scale;
			});

			SerializeComponent<CameraComponent>(entity, out, [&](auto& component) {
				out << Key << "Primary" << Value << component.Primary;
				out << Key << "FixedAspectRatio" << Value << component.FixedAspectRatio;

				out << Key << "Data";
				out << BeginMap; // Camera Values
					out << Key << "ProjectionType" << Value << (short)component.Camera.GetProjectionType();
					// Perspective
					out << Key << "PerspectiveFOV" << Value << component.Camera.GetPerspectiveFOV();
					out << Key << "PerspectiveNear" << Value << component.Camera.GetPerspectiveNearClip();
					out << Key << "PerspectiveFar" << Value << component.Camera.GetPerspectiveFarClip();
					// Orthographic
					out << Key << "OrthographicSize" << Value << component.Camera.GetOrthographicSize();
					out << Key << "OrthographicNear" << Value << component.Camera.GetOrthographicNearClip();
					out << Key << "OrthographicFar" << Value << component.Camera.GetOrthographicFarClip();
				out << EndMap; // Camera Values
			});

			SerializeComponent<SpriteRendererComponent>(entity, out, [&](auto& component) {
				out << Key << "Color" << Value << component.Color;
			});

		out << EndMap; // Entity
	}

	void SceneSerializer::Serialize(const std::string& filepath) {
		using namespace YAML;

		OC_CORE_TRACE("Serializing Scene To: '{0}'", filepath);

		Emitter out;
		out.SetIndent(4);
		out.SetMapFormat(Block);

		out << BeginMap; // Scene
			out << Key << "Scene" << Value << "Untitled";
			out << Key << "Entities" << Value;

			out << BeginSeq; // Registry
				for (const auto [ent, ref] : m_Scene->m_Registry.storage<TagComponent>().each()) {
					Entity entity = { ent, m_Scene.get() };

					if (!entity)
						return;

					SerializeEntity(out, entity);
					out << Newline;
				}
			out << EndSeq; // Registry
		out << EndMap; // Scene

		std::ofstream fout(filepath);
		fout << out.c_str();
	}

	void SceneSerializer::SerializeRuntime(const std::string& filepath) { }

	// Deserialization template for a single component.
	// 
	template<typename T, typename Function>
	static void DeserializeComponent(auto& entity, Entity& deserializedEntity, Function function) {
		auto entityComponent = entity[GetTypeName<T>()];

		if (entityComponent) {
			auto& component = deserializedEntity.GetOrAddComponent<T>();
			function(component, entityComponent);
		}
	}

	bool SceneSerializer::Deserialize(const std::string& filepath) {
		std::ifstream stream(filepath);
		std::stringstream strStream;
		strStream << stream.rdbuf();

		YAML::Node data = YAML::LoadFile(filepath);
		if (!data["Scene"])
			return false;

		std::string sceneName = data["Scene"].as<std::string>();

		OC_CORE_TRACE("Deserializing Scene '{0}'", sceneName);

		auto entities = data["Entities"];
		if (entities) {
			for (auto entity : entities) {
				uint64_t uuid = entity["Entity"].as<uint64_t>(); // TODO: UIDs (or GUIDs)

				std::string name;
				auto tagComponent = entity["TagComponent"];
				if (tagComponent)
					name = tagComponent["Tag"].as<std::string>();

				Entity deserializedEntity = m_Scene->CreateEntity(name);

				DeserializeComponent<TransformComponent>(entity, deserializedEntity, [&](auto& component, auto& data) {
					component.Translation = data["Translation"].as<glm::vec3>();
					component.Rotation = data["Rotation"].as<glm::vec3>();
					component.Scale = data["Scale"].as<glm::vec3>();
				});

				DeserializeComponent<CameraComponent>(entity, deserializedEntity, [&](auto& component, auto& data) {
					component.Primary = data["Primary"].as<bool>();
					component.FixedAspectRatio = data["FixedAspectRatio"].as<bool>();

					auto cameraData = data["Data"];
						component.Camera.SetProjectionType((SceneCamera::ProjectionType)cameraData["ProjectionType"].as<int>());

						component.Camera.SetPerspectiveFOV(cameraData["PerspectiveFOV"].as<float>());
						component.Camera.SetPerspectiveNearClip(cameraData["PerspectiveNear"].as<float>());
						component.Camera.SetPerspectiveFarClip(cameraData["PerspectiveFar"].as<float>());

						component.Camera.SetOrthographicSize(cameraData["OrthographicSize"].as<float>());
						component.Camera.SetOrthographicNearClip(cameraData["OrthographicNear"].as<float>());
						component.Camera.SetOrthographicFarClip(cameraData["OrthographicFar"].as<float>());
				});

				DeserializeComponent<SpriteRendererComponent>(entity, deserializedEntity, [&](auto& component, auto& data) {
					component.Color = data["Color"].as<glm::vec4>();
				});
			}
		}

		return true;
	}

	bool SceneSerializer::DeserializeRuntime(const std::string& filepath) {
		// TODO: Implement
		OC_CORE_ASSERT(false);
		return false;
	}

}	// Ocean
