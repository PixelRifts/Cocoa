#pragma once
#include "jade/core/Core.h"
#include "jade/util/Log.h"

#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>

namespace Jade
{
	namespace Interop
	{
		extern "C"
		{
			void _LogInfo(MonoString* message)
			{
				char* str = mono_string_to_utf8(message);
				Log::Info(str);
				mono_free(str);
			}

			void _LogWarning(MonoString* message)
			{
				char* str = mono_string_to_utf8(message);
				Log::Warning(str);
				mono_free(str);
			}

			void _LogError(MonoString* message)
			{
				char* str = mono_string_to_utf8(message);
				Log::ScriptError(str);
				mono_free(str);
			}
		}
	}
}