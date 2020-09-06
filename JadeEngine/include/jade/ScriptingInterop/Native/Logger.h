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
			void JADE _LogInfo(const char* message);
			void JADE _LogWarning(const char* message);
			void JADE _LogError(const char* message);
		}
	}
}