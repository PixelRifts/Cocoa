using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.CSharp.Syntax;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using JadeScriptRuntime;
using Microsoft.CodeAnalysis;

namespace JadeScriptCompiler.src
{
	class ClassCollector : CSharpSyntaxWalker
	{
		public List<string> Classes { get; } = new List<string>();
		private INamedTypeSymbol _baseClassType;
		private SemanticModel _context;

		public ClassCollector(INamedTypeSymbol baseClassToCheck, SemanticModel model)
		{
			_baseClassType = baseClassToCheck;
			_context = model;
		}

		public override void VisitClassDeclaration(ClassDeclarationSyntax node)
		{
			if (InheritsFromBaseClass(_context.GetDeclaredSymbol(node).BaseType))
			{
				this.Classes.Add(node.Identifier.ToString());
			}
		}

		private bool InheritsFromBaseClass(ITypeSymbol symbol)
		{
			var baseType = symbol;
			while (baseType != null)
			{
				if (_baseClassType.Equals(_baseClassType))
				{
					return true;
				}
				baseType = baseType.BaseType;
			}
			return false;
		}
	}
}
