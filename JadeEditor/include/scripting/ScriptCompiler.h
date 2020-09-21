#pragma once
#include "externalLibs.h"

#include "jade/util/Log.h"
#include "jade/file/JPath.h"

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>

namespace Jade
{
	class ScriptCompiler
	{
	public:
		static void Init();
		static void Compile(const JPath& pathToScript, const JPath& pathToOutput);

	private:
		static MonoImage* s_CompilerImage;
		static MonoMethod* s_CompileMethod;
	};
}