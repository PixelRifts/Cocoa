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
			void JADE _LogInfo(MonoString* message);
			void JADE _LogWarning(MonoString* message);
			void JADE _LogError(MonoString* message);
		}
	}
}