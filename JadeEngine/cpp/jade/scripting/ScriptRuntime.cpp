#include "jade/scripting/ScriptRuntime.h"
#include "jade/scripting/Logger.h"
#include "jade/util/Settings.h"

#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/mono-gc.h>

namespace Jade
{
	MonoDomain* ScriptRuntime::s_Domain = nullptr;

	void ScriptRuntime::Init()
	{
		mono_set_dirs("C:\\dev\\C++\\MonoTEST\\MonoTest\\monoVendor\\lib", "C:\\dev\\C++\\MonoTEST\\MonoTest\\monoVendor\\etc");

		// Initialize the root app domain
		s_Domain = mono_jit_init("JadeEngineScriptRuntime");
	}

	void ScriptRuntime::Destroy()
	{
		mono_jit_cleanup(s_Domain);
	}

	void ScriptRuntime::OnSceneInit(Scene& scene)
	{
		JPath jadeScriptsDll = Settings::General::s_EngineExecutableDirectory + "JadeScripts.dll";
		m_CurrentExecutingDomain = mono_domain_create_appdomain("ScriptRuntimeDomain", NULL);
		mono_domain_set(m_CurrentExecutingDomain, false);

		MonoAssembly* assembly = mono_domain_assembly_open(m_CurrentExecutingDomain, jadeScriptsDll.Filepath());
		if (!assembly)
		{
			Log::ScriptError("Failed to load assembly for script dll '%s'", jadeScriptsDll.Filepath());
			return;
		}

		MonoImageOpenStatus status;
		m_CurrentExecutingImage = mono_assembly_get_image(assembly);

		if (!m_CurrentExecutingImage)
		{
			Log::ScriptError("Failed to get compiled assembly image for script dll '%s'", jadeScriptsDll.Filepath());
			return;
		}

		entt::registry& reg = scene.GetRegistry();
		auto scriptView = reg.view<ScriptableComponent>();
		for (auto entity : scriptView)
		{
			ScriptableComponent& script = reg.get<ScriptableComponent>(entity);
			MonoClass* klazz = mono_class_from_name(m_CurrentExecutingImage, "", script.GetMetadata()->GetClassName().c_str());
			MonoObject* obj = mono_object_new(m_CurrentExecutingDomain, klazz);
			mono_runtime_object_init(obj);

			MonoClass* parent = mono_class_get_parent(klazz);

			MonoMethod* bindMethod = mono_class_get_method_from_name(parent, "BindFunctions", 1);
			if (!bindMethod)
			{
				Log::ScriptError("Failed to find BindFunctions method in C# script '%s'. \n\tDoes this script inherit from ScriptRuntime?", script.GetFilepath());
				return;
			}

			ScriptableComponent* scriptPtr = &script;
			void* args[1]{ &scriptPtr };
			mono_runtime_invoke(bindMethod, obj, args, nullptr);
		}
	}

	void ScriptRuntime::OnSceneUpdate(Scene& scene, float dt)
	{
		scene.GetRegistry().view<ScriptableComponent>().each([dt](auto entity, ScriptableComponent& script)
		{
			script.Update(dt);
		});
	}

	void ScriptRuntime::OnSceneStop(Scene& scene)
	{
		if (m_CurrentExecutingDomain && m_CurrentExecutingDomain != mono_get_root_domain())
		{
			mono_domain_set(mono_get_root_domain(), false);
			mono_domain_unload(m_CurrentExecutingDomain);
		}
	}

	void ScriptRuntime::OnSceneStart(Scene& scene)
	{
		scene.GetRegistry().view<ScriptableComponent>().each([](auto entity, ScriptableComponent& script)
		{
			script.Start();
		});
	}

	void ScriptRuntime::Serialize(json& j, const Entity& entity, const ScriptableComponent& script)
	{
		json assetId = { "MetadataAssetId", script.GetMetadata()->GetResourceId() };

		int size = j["Size"];
		j["Components"][size] = {
			{"ScriptableComponent", {
				{"Entity", entity.GetID()},
				assetId
			}}
		};

		j["Size"] = size + 1;
	}

	void ScriptRuntime::Deserialize(json& j, Entity entity)
	{
		uint32 assetId = j["ScriptableComponent"]["MetadataAssetId"];
		std::shared_ptr<ScriptMetadata> metadata = std::static_pointer_cast<ScriptMetadata>(AssetManager::GetAsset(assetId));
		entity.AddComponent<ScriptableComponent>(metadata);
	}
}