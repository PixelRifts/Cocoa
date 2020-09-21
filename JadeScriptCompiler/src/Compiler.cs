using Microsoft.CSharp;
using System.IO;
using System.CodeDom.Compiler;
using JadeScriptRuntime;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using JadeScriptCompiler.src;
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
					StreamReader stream = new StreamReader(script);
					string code = stream.ReadToEnd();
					stream.Close();
					SyntaxTree tree = CSharpSyntaxTree.ParseText(code);
					CompilationUnitSyntax root = tree.GetCompilationUnitRoot();

					var executableReference = MetadataReference.CreateFromFile(pathToRuntimeDll);
					var compilation = CSharpCompilation.Create(results.CompiledAssembly.GetName().ToString())
						.AddReferences(executableReference)
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
	}
}
