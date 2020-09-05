#include "jade/ScriptingInterop/Native/ScriptRuntime.h"
#include "jade/ScriptingInterop/Native/Test.h"
#include "jade/ScriptingInterop/Native/Logger.h"

namespace Jade
{
	MonoDomain* ScriptRuntime::s_Domain = nullptr;

	void ScriptRuntime::Init()
	{
		mono_set_dirs("C:\\dev\\C++\\MonoTEST\\MonoTest\\monoVendor\\lib", "C:\\dev\\C++\\MonoTEST\\MonoTest\\monoVendor\\etc");

		s_Domain = mono_jit_init("JadeEngineScriptRuntime");
		JPath scriptRuntime = "C:/dev/C++/JadeEngine/bin/Debug-windows-x86_64/JadeEditor/JadeScriptRuntime.exe";
		AddInternalCallsToCSharp();
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
		JPath scriptRuntime = "C:/dev/C++/JadeEngine/bin/Debug-windows-x86_64/JadeEditor/JadeScriptRuntime.exe";
		MonoImageOpenStatus status;
		MonoAssembly* assembly = mono_assembly_open(scriptRuntime.Filepath(), &status);
		if (!assembly)
		{
			Log::Error("Failed to load compiled C# file: %s", scriptRuntime.Filepath());
		}

		return assembly;
	}

	void ScriptRuntime::CallCSharpMethod(MonoAssembly* assembly, MonoClass* klazz, const std::string& methodName)
	{
		Log::Warning("TODO: Implement me!");
	}

	void ScriptRuntime::AddInternalCallsToCSharp()
	{
		//mono_add_internal_call("JadeScriptRuntime.Jade::Init", &Interop::Init);
		mono_add_internal_call("JadeScriptRuntime.Debug::_LogInfo", &Interop::_LogInfo1);
		mono_add_internal_call("JadeScriptRuntime.Debug::_LogWarning", &Interop::_LogWarning1);
		mono_add_internal_call("JadeScriptRuntime.Debug::_LogError", &Interop::_LogError1);
	}
}