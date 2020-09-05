#pragma once
#include "externalLibs.h"
#include "jade/file/JPath.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Jade
{
	class ScriptRuntime
	{
	public:
		static void Init();
		static void Destroy();

		static MonoAssembly* OpenCSharpExe(JPath pathToExe);
		static void CallCSharpMethod(MonoAssembly* assembly, MonoClass* klazz, const std::string& methodName);

	private:
		static void AddInternalCallsToCSharp();
		static MonoDomain* s_Domain;
	};
}