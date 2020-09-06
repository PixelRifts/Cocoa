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
		static void Compile(string pathToFile, string outputPath)
		{
			Debug.LogInfo($"Path to File: {pathToFile}");
			Debug.LogInfo($"Output path: {outputPath}");
			var codeProvider = new CSharpCodeProvider();

			var parameters = new CompilerParameters();
			parameters.ReferencedAssemblies.Add("C:/dev/C++/JadeEngine/bin/Debug-windows-x86_64/JadeScriptRuntime/JadeScriptRuntime.exe");

			var results = codeProvider.CompileAssemblyFromFile(parameters, pathToFile);

			if (results.Errors.Count > 0)
			{
				foreach (CompilerError CompError in results.Errors)
				{
					Console.WriteLine($"Line Number: {CompError.Line}, Error Number: {CompError.ErrorNumber}, '{CompError.ErrorText}'");
				}
			}
			else
			{
				File.Copy(results.PathToAssembly, outputPath, true);
				Debug.LogInfo($"Output path: {results.PathToAssembly}");
				File.Delete(results.PathToAssembly);

			}
		}

		static void DoesItWork()
		{
			Debug.LogInfo("Hello there. It does indeed work");
		}

		static void Main(string[] args)
		{
			Debug.LogInfo("Well we can init at least...");
		}
	}
}
