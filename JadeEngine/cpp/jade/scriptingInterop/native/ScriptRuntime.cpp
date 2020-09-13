#include "jade/ScriptingInterop/Native/ScriptRuntime.h"
#include "jade/ScriptingInterop/Native/Test.h"
#include "jade/ScriptingInterop/Native/Logger.h"
#include "jade/util/Settings.h"

#include <mono/metadata/debug-helpers.h>

namespace Jade
{
	MonoDomain* ScriptRuntime::s_Domain = nullptr;

	void ScriptRuntime::Init()
	{
		mono_set_dirs("C:\\dev\\C++\\MonoTEST\\MonoTest\\monoVendor\\lib", "C:\\dev\\C++\\MonoTEST\\MonoTest\\monoVendor\\etc");

		s_Domain = mono_jit_init("JadeEngineScriptRuntime");
		JPath scriptRuntime = "C:/dev/C++/JadeEngine/bin/Debug-windows-x86_64/JadeEditor/JadeScriptRuntime.exe";
		MonoAssembly* assembly = OpenCSharpExe(scriptRuntime);

		int argc = 1;
		char* argv[1] = { (char*)"MyDomain" };
		int returnVal = mono_jit_exec(s_Domain, assembly, argc, argv);
	}

	void ScriptRuntime::Destroy()
	{
		mono_jit_cleanup(s_Domain);
	}

	MonoAssembly* ScriptRuntime::OpenCSharpExe(JPath pathToExe)
	{
		MonoImageOpenStatus status;
		MonoAssembly* assembly = mono_assembly_open(pathToExe.Filepath(), &status);
		if (!assembly)
		{
			Log::Error("Failed to load compiled C# file: %s", pathToExe.Filepath());
		}

		return assembly;
	}

	void ScriptRuntime::OnSceneInit(Scene& scene)
	{
		JPath jadeScriptsDll = Settings::General::s_EngineExecutableDirectory + "JadeScripts.dll";
		m_CurrentExecutingDomain = mono_domain_create();
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
			MonoClass* klazz = mono_class_from_name(m_CurrentExecutingImage, "", "MyScript");// script.GetFilepath().Filename());
			MonoObject* obj = mono_object_new(m_CurrentExecutingDomain, klazz);
			mono_runtime_object_init(obj);

			void* iter = nullptr;
			MonoMethod* m = nullptr;
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
		mono_image_close(m_CurrentExecutingImage);
		mono_domain_free(m_CurrentExecutingDomain, true);
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
		json path = { "Path", script.GetFilepath().Filepath() };

		int size = j["Size"];
		j["Components"][size] = {
			{"ScriptableComponent", {
				{"Entity", entity.GetID()},
				path
			}}
		};

		j["Size"] = size + 1;
	}

	void ScriptRuntime::Deserialize(json& j, Entity entity)
	{
		JPath path = j["ScriptableComponent"]["Path"];
		entity.AddComponent<ScriptableComponent>(path);
	}

	void ScriptRuntime::ExecuteScriptableComponent(const JPath& path)
	{
		MonoDomain* newDomain = mono_domain_create();
		MonoAssembly* assembly = mono_domain_assembly_open(newDomain, path.Filepath());
		if (!assembly)
		{
			Log::ScriptError("Failed to load assembly for script dll.");
			return;
		}

		MonoImageOpenStatus status;
		MonoImage* s_CompilerImage = mono_assembly_get_image(assembly);

		if (!s_CompilerImage)
		{
			Log::ScriptError("Failed to open script dll.");
			return;
		}

		MonoClass* klazz = mono_class_from_name(s_CompilerImage, "", "RandomComponent");
		MonoObject* obj = mono_object_new(newDomain, klazz);
		mono_runtime_object_init(obj);

		void* iter = nullptr;
		MonoMethod* m = nullptr;
		MonoClass* parent = mono_class_get_parent(klazz);

		MonoMethodDesc* bindDesc = mono_method_desc_new(":ToString()", false);
		MonoMethod* bindMethod = mono_class_get_method_from_name(parent, "BindFunctions", 1);
		mono_method_desc_free(bindDesc);

		if (!bindMethod)
		{
			Log::ScriptError("Failed to find BindFunctions method in C# script.");
			return;
		}

		ScriptableComponent* testComp = new ScriptableComponent(path);
		void* args[1]{&testComp};
		mono_runtime_invoke(bindMethod, obj, args, nullptr);

		mono_image_close(s_CompilerImage);
		mono_domain_free(newDomain, true);

		testComp->Start();
		testComp->Update(0.5f);
	}

	void ScriptRuntime::CallCSharpMethod(MonoAssembly* assembly, MonoClass* klazz, const std::string& methodName)
	{
		Log::Warning("TODO: Implement me!");
	}
}