#include "ScriptingInterop/Native/ScriptCompiler.h"
#include "jade/ScriptingInterop/Native/ScriptRuntime.h"

#include <mono/metadata/debug-helpers.h>

namespace Jade
{
	MonoImage* ScriptCompiler::s_CompilerImage = nullptr;
	MonoMethod* ScriptCompiler::s_CompileMethod = nullptr;

	static MonoMethod* testMethod = nullptr;
	static MonoDomain* s_Domain = nullptr;
	void ScriptCompiler::Init()
	{
		s_Domain = ScriptRuntime::s_Domain;
		//s_Domain = mono_jit_init("C:/dev/C++/JadeEngine/bin/Debug-windows-x86_64/JadeEditor/JadeScriptCompiler.exe");
		
		MonoAssembly* assembly = mono_domain_assembly_open(s_Domain, "C:/dev/C++/JadeEngine/bin/Debug-windows-x86_64/JadeEditor/JadeScriptCompiler.exe");
		if (!assembly)
		{
			Log::Error("Failed to load assembly.");
		}

		int argc = 1;
		char* argv[1] = { (char*)"MyDomain" };
		mono_jit_exec(s_Domain, assembly, argc, argv);

		MonoImageOpenStatus status;
		s_CompilerImage = mono_assembly_get_image(assembly);

		if (!s_CompilerImage)
		{
			Log::Error("Failed to open C# compiler.");
		}

		MonoClass* klazz = mono_class_from_name(s_CompilerImage, "JadeScriptCompiler", "Compiler");
		MonoMethodDesc* desc = mono_method_desc_new("JadeScriptCompiler.Compiler:Compile(string,string)", false);
		s_CompileMethod = mono_method_desc_search_in_class(desc, klazz);
		mono_method_desc_free(desc);

		if (!s_CompileMethod)
		{
			Log::Error("Failed to find compile method in C# compiler.");
		}
	}

	void ScriptCompiler::Compile(const JPath& pathToScript, const JPath& pathToOutput)
	{
		void* args[2];
		
		MonoString* monoInputPath = mono_string_new(s_Domain, pathToScript.Filepath());
		MonoString* monoOutputPath = mono_string_new(s_Domain, pathToOutput.Filepath());

		args[0] = monoInputPath;
		args[1] = monoOutputPath;

		//mono_runtime_invoke(testMethod, NULL, NULL, NULL);

		//MonoError error;
		mono_runtime_invoke(s_CompileMethod, nullptr, args, nullptr);
	}
}