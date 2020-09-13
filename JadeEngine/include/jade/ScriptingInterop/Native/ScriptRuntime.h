#pragma once
#include "externalLibs.h"
#include "jade/file/JPath.h"
#include "jade/scenes/Scene.h"
#include "jade/components/ScriptableComponent.h"
#include "jade/core/Entity.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

#include <nlohmann/json.hpp>

namespace Jade
{
	class JADE ScriptRuntime
	{
	public:
		static void Init();
		static void Destroy();

		static MonoAssembly* OpenCSharpExe(JPath pathToExe);
		static void CallCSharpMethod(MonoAssembly* assembly, MonoClass* klazz, const std::string& methodName);
		void OnSceneInit(Scene& scene);
		void OnSceneUpdate(Scene& scene, float dt);
		void OnSceneStart(Scene& scene);
		void OnSceneStop(Scene& scene);

		static void ExecuteScriptableComponent(const JPath& path);
		static void Serialize(json& j, const Entity& entity, const ScriptableComponent& script);
		static void Deserialize(json& j, Entity entity);

		static MonoDomain* s_Domain;
	private:
		static void LoadScriptableComponent(const JPath& path, ScriptableComponent& script);

		MonoDomain* m_CurrentExecutingDomain = nullptr;
		MonoImage* m_CurrentExecutingImage = nullptr;
	};
}