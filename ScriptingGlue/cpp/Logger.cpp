#pragma once
#include "jade/core/Core.h"
#include "jade/util/Log.h"

#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>

namespace Jade
{
	namespace Glue
	{
		extern "C"
		{
			JADE void _LogInfo(MonoString* message)
			{
				char* str = mono_string_to_utf8(message);
				Log::Info("Calling from new dll...");
				Log::Info(str);
				mono_free(str);
			}

			JADE void _LogWarning(MonoString* message)
			{
				char* str = mono_string_to_utf8(message);
				Log::Warning(str);
				mono_free(str);
			}

			JADE void _LogError(MonoString* message)
			{
				char* str = mono_string_to_utf8(message);
				Log::ScriptError(str);
				mono_free(str);
			}
		}
	}
}