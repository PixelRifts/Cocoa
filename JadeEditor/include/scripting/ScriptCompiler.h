#pragma once
#include "externalLibs.h"

#include "jade/util/Log.h"
#include "jade/file/JPath.h"
#include "jade/scripting/ScriptRuntime.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Jade
{
	class ScriptCompiler
	{
	public:
		static void Init(ScriptRuntime* scriptRuntime);
		static void LoadCSharp();
		static void Compile(const JPath& pathToScript, const JPath& pathToOutput);
		static std::string GetClassName(const JPath& pathToScript);

		static void EditorInit(Scene& scene);
		static void EditorStop();

	private:
		static ScriptRuntime* s_ScriptRuntime;
		static MonoImage* s_CompilerImage;
		static MonoMethod* s_CompileMethod;
		static MonoMethod* s_GetClassNameMethod;
	};
}