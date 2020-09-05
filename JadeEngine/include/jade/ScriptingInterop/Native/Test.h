#pragma once
#include "jade/core/Core.h"
#include "jade/util/Log.h"

namespace Jade
{
	namespace Interop
	{
		extern "C"
		{
			void(*PrintMessage)();

			JADE void Init(void(*csharpFunctionPtr)())
			{
				PrintMessage = csharpFunctionPtr;
			}

			void TestCall()
			{
				PrintMessage();
			}
		}
	}
}