using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;
using System.Runtime.CompilerServices;

namespace JadeScriptRuntime
{
	public class JImGui
	{
		// -------------------------------------------------------
		// Native code declarations
		// -------------------------------------------------------
		[DllImport("__Internal", EntryPoint = "_UndoableDragFloat3", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
		unsafe static extern void _UndoableDragFloat3(
			[MarshalAs(UnmanagedType.LPStr)]string label, 
			[MarshalAs(UnmanagedType.LPArray)]float[] floatPtr);

		[DllImport("__Internal", EntryPoint = "_UndoableDragFloat2", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
		unsafe static extern void _UndoableDragFloat2(
			[MarshalAs(UnmanagedType.LPStr)]string label, 
			[MarshalAs(UnmanagedType.LPArray)]float[] floatPtr);

		[DllImport("__Internal", EntryPoint = "_UndoableDragFloat", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
		unsafe static extern void _UndoableDragFloat(
			[MarshalAs(UnmanagedType.LPStr)]string label,
			[MarshalAs(UnmanagedType.LPArray)]ref float floatPtr);

		// -------------------------------------------------------
		// Wrappers around native code calls
		// -------------------------------------------------------
		public static void UndoableDragFloat3(string label, float[] vector)
		{
			Debug.LogInfo($"ImGui drag float 3: {vector[0]}, {vector[1]}, {vector[2]}");
			_UndoableDragFloat3(label, vector);
		}

		public static void UndoableDragFloat2(string label, float[] vector)
		{
			Debug.LogInfo($"ImGui drag float 2: {vector[0]}, {vector[1]}");
			_UndoableDragFloat2(label, vector);
		}

		public static void UndoableDragFloat(string label, ref float floatRef)
		{
			Debug.LogInfo($"ImGui drag float: {floatRef}");
			_UndoableDragFloat(label, ref floatRef);
		}
	}
}
