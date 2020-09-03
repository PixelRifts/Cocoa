using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace JadeScriptRuntime
{
	class Jade
	{
		[MethodImpl(MethodImplOptions.InternalCall)]
		static extern void Init(IntPtr csharpFunctionPtr);

		public static void PrintMessage()
		{
			Debug.LogInfo("Hello world! From C# :D");
			Debug.LogWarning("Uh oh, I just logged a warning!");
			Debug.LogError("Double uh oh. This is an error coming from a script... What to do, what to do.");
		}

		public static void InitPlugin()
		{
			Debug.LogInfo("Initializing C# functions pointers in C++.");
			Debug.LogInfo("C++ should not be calling into C# other than calling the update/start/etc functions on Scripts.");
			Action _delegate = new Action(PrintMessage);

			IntPtr funcPtr = Marshal.GetFunctionPointerForDelegate(_delegate);

			Init(funcPtr);
		}
	}
}
