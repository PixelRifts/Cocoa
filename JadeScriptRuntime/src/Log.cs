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
		[DllImport("JadeEngine", EntryPoint = "_LogInfo", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
		static extern void _LogInfo(
			[MarshalAs(UnmanagedType.LPStr)]string message);

		[DllImport("JadeEngine", EntryPoint = "_LogWarning", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
		static extern void _LogWarning(
			[MarshalAs(UnmanagedType.LPStr)]string message);

		[DllImport("JadeEngine", EntryPoint = "_LogError", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
		static extern void _LogError(
			[MarshalAs(UnmanagedType.LPStr)]string message);

		// -------------------------------------------------------
		// Wrappers around native code calls
		// -------------------------------------------------------
		public static void LogInfo(string message)
		{
			_LogInfo(message);
		}

		public static void LogWarning(string message)
		{
			_LogWarning(message);
		}

		public static void LogError(string message)
		{
			_LogError(message);
		}
	}
}
