#pragma once
#include "jade/core/Core.h"
#include "jade/file/JPath.h"

namespace Jade
{
	typedef void(*ScriptStartFnPt)();
	typedef void(*ScriptUpdateFnPt)(float dt);

	class JADE ScriptableComponent
	{
	public:
		ScriptableComponent(const JPath& filepath)
			: m_Filepath(filepath) {}

		ScriptableComponent(ScriptableComponent&& other)
			: m_ScriptStart(std::move(other.m_ScriptStart)), m_ScriptUpdate(std::move(other.m_ScriptUpdate)), m_Filepath(std::move(other.m_Filepath)) {}

		void Start();
		void Update(float dt);

		void SetStartFnPt(ScriptStartFnPt startFnPt) { m_ScriptStart = startFnPt; }
		void SetUpdateFnPt(ScriptUpdateFnPt updateFnPt) { m_ScriptUpdate = updateFnPt; }

		inline const JPath& GetFilepath() { return m_Filepath; }

		ScriptableComponent& operator=(ScriptableComponent&& other) noexcept
		{
			m_ScriptStart = std::move(other.m_ScriptStart);
			m_ScriptUpdate = std::move(other.m_ScriptUpdate);
			m_Filepath = std::move(other.m_Filepath);
			return *this;
		}

	private:
		ScriptStartFnPt m_ScriptStart;
		ScriptUpdateFnPt m_ScriptUpdate;

		JPath m_Filepath;
	};

	extern "C" JADE void InitScript(ScriptableComponent* scriptObj, ScriptStartFnPt scriptStart, ScriptUpdateFnPt scriptUpdate);
}