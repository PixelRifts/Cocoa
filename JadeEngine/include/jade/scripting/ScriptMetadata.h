#pragma once

#include "jade/core/AssetManager.h"

namespace Jade
{
	class JADE ScriptMetadata : public Asset
	{
	public:
		ScriptMetadata(const JPath& filepath, const std::string& className)
			: m_ClassName(className)
		{
			GenerateTypeId<ScriptMetadata>();
			m_Path = filepath;
		}

		virtual void Load() override {}
		virtual void Unload() override {}

		inline const JPath& GetFilepath() const { return m_Path; }
		inline const std::string& GetClassName() const { return m_ClassName; }
		inline void SetClassName(char* newName) { m_ClassName = newName; }

	private:
		std::string m_ClassName;
	};
}