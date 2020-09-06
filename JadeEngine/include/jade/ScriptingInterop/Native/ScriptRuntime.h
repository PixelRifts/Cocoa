#pragma once
#include "externalLibs.h"
#include "jade/file/JPath.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Jade
{
	class JADE ScriptRuntime
	{
	public:
		static void Init();
		static void Destroy();

		static MonoAssembly* OpenCSharpExe(JPath pathToExe);
		static void CallCSharpMethod(MonoAssembly* assembly, MonoClass* klazz, const std::string& methodName);

		static void ExecuteScriptableComponent(const JPath& path);

		static MonoDomain* s_Domain;
	private:
	};
}