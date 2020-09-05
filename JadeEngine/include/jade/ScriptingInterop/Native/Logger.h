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
			JADE void _LogInfo(MonoString* message);
			JADE void _LogWarning(MonoString* message);
			JADE void _LogError(MonoString* message);
		}
	}
}