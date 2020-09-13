using Microsoft.CSharp;
using System;
using System.IO;
using System.CodeDom.Compiler;
using System.Runtime.InteropServices;
using JadeScriptRuntime;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using System.Linq;
using JadeScriptCompiler.src;
using System.Reflection;
using System.Text;
using System.Collections.Generic;

namespace JadeScriptCompiler
{
	class Compiler
	{
		static string[] Compile(string pathToScripts, string outputPath, string pathToRuntimeDll)
		{
			var codeProvider = new CSharpCodeProvider();

			var parameters = new CompilerParameters();
			parameters.ReferencedAssemblies.Add(pathToRuntimeDll);
			parameters.OutputAssembly = outputPath;

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
				List<string> res = new List<string>();
				// Get class names for each file compiled
				foreach (var script in scripts)
				{
					var code = new StreamReader(script).ReadToEnd();
					SyntaxTree tree = CSharpSyntaxTree.ParseText(code);
					CompilationUnitSyntax root = tree.GetCompilationUnitRoot();

					var compilation = CSharpCompilation.Create(results.CompiledAssembly.GetName().ToString())
						.AddReferences(MetadataReference.CreateFromFile(pathToRuntimeDll))
						.AddSyntaxTrees(tree);

					var baseType = compilation.GetTypeByMetadataName("JadeScriptRuntime.ScriptableComponent");
					var classCollector = new ClassCollector(baseType, compilation.GetSemanticModel(tree));
					classCollector.Visit(root);

					for (int i = 0; i < classCollector.Classes.Count; i++)
					{
						res.Add(script);
						res.Add(classCollector.Classes[i]);
					}
				}
				return res.ToArray();
			}

			return null;
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
