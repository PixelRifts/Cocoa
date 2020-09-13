using Microsoft.CSharp;
using System;
using System.IO;
using System.CodeDom.Compiler;
using System.Runtime.InteropServices;
using JadeScriptRuntime;

namespace JadeScriptCompiler
{
	class Compiler
	{
		static void Compile(string pathToScripts, string outputPath, string pathToRuntimeDll)
		{
			var codeProvider = new CSharpCodeProvider();

			var parameters = new CompilerParameters();
			parameters.ReferencedAssemblies.Add(pathToRuntimeDll);

			var scripts = Directory.GetFiles(pathToScripts);
			var results = codeProvider.CompileAssemblyFromFile(parameters, scripts);


			if (results.Errors.Count > 0)
			{
				foreach (CompilerError CompError in results.Errors)
				{
					Debug.LogError($"File: '{CompError.FileName}'\n\tError Number: {CompError.ErrorNumber}\n\tLine Number: {CompError.Line}, '{CompError.ErrorText}'");
				}
			}
			else
			{
				File.Copy(results.PathToAssembly, outputPath, true);
				File.Delete(results.PathToAssembly);
			}
		}

		static void DoesItWork()
		{
			Debug.LogInfo("Hello there. It does indeed work");
		}

		static void Main(string[] args)
		{
		}
	}
}
