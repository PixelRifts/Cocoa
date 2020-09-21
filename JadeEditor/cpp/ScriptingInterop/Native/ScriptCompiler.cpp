#include "ScriptingInterop/Native/ScriptCompiler.h"
#include "jade/ScriptingInterop/Native/ScriptRuntime.h"
#include "jade/util/Settings.h"

#include <mono/metadata/debug-helpers.h>
#include <mono/metadata/exception.h>

namespace Jade
{
	MonoImage* ScriptCompiler::s_CompilerImage = nullptr;
	MonoMethod* ScriptCompiler::s_CompileMethod = nullptr;

	static MonoDomain* s_Domain = nullptr;

	void ScriptCompiler::Init()
	{
		s_Domain = mono_domain_create_appdomain("JadeScriptCompiler", NULL);
		mono_domain_set(s_Domain, false);

		MonoAssembly* assembly = mono_domain_assembly_open(s_Domain, "C:/dev/C++/JadeEngine/bin/Debug-windows-x86_64/JadeEditor/JadeScriptCompiler.exe");
		if (!assembly)
		{
			Log::Error("Failed to load assembly.");
		}

		MonoImageOpenStatus status;
		s_CompilerImage = mono_assembly_get_image(assembly);

		if (!s_CompilerImage)
		{
			Log::Error("Failed to open C# compiler.");
		}

		MonoClass* klazz = mono_class_from_name(s_CompilerImage, "JadeScriptCompiler", "Compiler");
		MonoMethodDesc* desc = mono_method_desc_new("JadeScriptCompiler.Compiler:Compile(string,string,string)", false);
		s_CompileMethod = mono_method_desc_search_in_class(desc, klazz);
		mono_method_desc_free(desc);

		if (!s_CompileMethod)
		{
			Log::Error("Failed to find compile method in C# compiler.");
		}
	}

	void ScriptCompiler::Compile(const JPath& pathToScripts, const JPath& pathToOutput)
	{
		Init();
		void* args[3];

		MonoString* monoPathToScripts = mono_string_new(s_Domain, pathToScripts.Filepath());
		MonoString* monoOutputPath = mono_string_new(s_Domain, pathToOutput.Filepath());
		MonoString* monoPathToScriptRuntimeDll = mono_string_new(s_Domain, (Settings::General::s_EngineExecutableDirectory + "JadeScriptRuntime.dll").Filepath());

		args[0] = monoPathToScripts;
		args[1] = monoOutputPath;
		args[2] = monoPathToScriptRuntimeDll;

		MonoError error;
		MonoArray* res = (MonoArray*)mono_runtime_invoke(s_CompileMethod, nullptr, args, nullptr);

		if (res != nullptr)
		{
			uintptr_t arrayLength = mono_array_length(res);
			for (int i = 0; i < arrayLength; i++)
			{
				MonoString* element = mono_array_get(res, MonoString*, i);
				char* cStr = mono_string_to_utf8(element);
				Log::Info("C++ code says: '%s'", cStr);
				mono_free(cStr);
			}
		}

		MonoDomain* domainToUnload = mono_domain_get();
		if (domainToUnload && domainToUnload != mono_get_root_domain())
		{
			mono_domain_set(mono_get_root_domain(), false);
			mono_domain_unload(domainToUnload);
		}
	}
}