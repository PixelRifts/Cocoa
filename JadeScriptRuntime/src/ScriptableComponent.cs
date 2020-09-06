using System.Runtime.InteropServices;
using System;
using System.CodeDom.Compiler;

namespace JadeScriptRuntime
{
	public abstract class ScriptableComponent
	{
		[DllImport("JadeEngine", EntryPoint = "InitScript", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
		static extern void Init(IntPtr scriptObj, IntPtr startFn, IntPtr updateFn);

		public void BindFunctions(IntPtr scriptObj)
		{
			Debug.LogInfo($"{scriptObj}");
			Action _startDelegate = new Action(Start);
			Action<float> _updateDelegate = new Action<float>(Update);

			IntPtr startFnPt = Marshal.GetFunctionPointerForDelegate(_startDelegate);
			IntPtr updateFnPt = Marshal.GetFunctionPointerForDelegate(_updateDelegate);
			Init(scriptObj, startFnPt, updateFnPt);
		}

		virtual public void Start()
		{

		}

		virtual public void Update(float dt)
		{

		}
	}
}
