#pragma once
#include "jade/core/Core.h"

namespace Jade
{
	typedef void(*ScriptStartFnPt)();
	typedef void(*ScriptUpdateFnPt)(float dt);

	JADE class ScriptableComponent
	{
	public:
		ScriptableComponent() = default;
		void Start();
		void Update(float dt);

		void SetStartFnPt(ScriptStartFnPt startFnPt) { m_ScriptStart = startFnPt; }
		void SetUpdateFnPt(ScriptUpdateFnPt updateFnPt) { m_ScriptUpdate = updateFnPt; }

	private:
		ScriptStartFnPt m_ScriptStart;
		ScriptUpdateFnPt m_ScriptUpdate;
	};

	extern "C" JADE void InitScript(ScriptableComponent & scriptObj, ScriptStartFnPt scriptStart, ScriptUpdateFnPt scriptUpdate);
}