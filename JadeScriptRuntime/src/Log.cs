using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace JadeScriptRuntime
{
	public class Debug
	{
		// -------------------------------------------------------
		// Native code declarations
		// -------------------------------------------------------
		[DllImport("ScriptingGlue", EntryPoint = "_LogInfo", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
		//[MethodImpl(MethodImplOptions.InternalCall)]
		static extern void _LogInfo([MarshalAs(UnmanagedType.LPStr)]string message);

		[MethodImpl(MethodImplOptions.InternalCall)]
		static extern void _LogWarning(string message);

		[MethodImpl(MethodImplOptions.InternalCall)]
		static extern void _LogError(string message);

		// -------------------------------------------------------
		// Wrappers around native code calls
		// -------------------------------------------------------
		public static void LogInfo(string message)
		{
			System.Diagnostics.StackFrame stackFrame = new System.Diagnostics.StackTrace().GetFrame(1);
			_LogInfo($"{stackFrame.GetMethod()}: {message}");
		}

		public static void LogWarning(string message)
		{
			System.Diagnostics.StackFrame stackFrame = new System.Diagnostics.StackTrace().GetFrame(1);
			_LogWarning($"{stackFrame.GetMethod()}: {message}");
		}

		public static void LogError(string message)
		{
			System.Diagnostics.StackFrame stackFrame = new System.Diagnostics.StackTrace().GetFrame(1);
			_LogError($"{stackFrame.GetMethod()}: {message}");
		}
	}
}
