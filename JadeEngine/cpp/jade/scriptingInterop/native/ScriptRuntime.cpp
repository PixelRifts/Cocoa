#include "jade/ScriptingInterop/Native/ScriptRuntime.h"
#include "jade/ScriptingInterop/Native/Test.h"
#include "jade/ScriptingInterop/Native/Logger.h"
#include "jade/components/ScriptableComponent.h"

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

		ScriptableComponent* testComp = new ScriptableComponent();
		void* args[1]{&testComp};
		mono_runtime_invoke(bindMethod, obj, args, nullptr);

		testComp->Start();
		testComp->Update(0.5f);

		mono_image_close(s_CompilerImage);
		mono_domain_free(newDomain, true);
	}

	void ScriptRuntime::CallCSharpMethod(MonoAssembly* assembly, MonoClass* klazz, const std::string& methodName)
	{
		Log::Warning("TODO: Implement me!");
	}
}