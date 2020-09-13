#include "jade/components/ScriptableComponent.h"
#include "jade/util/Log.h"

namespace Jade
{
	extern "C" JADE void InitScript(ScriptableComponent* scriptObj, ScriptStartFnPt scriptStart, ScriptUpdateFnPt scriptUpdate)
	{
		scriptObj->SetStartFnPt(scriptStart);
		scriptObj->SetUpdateFnPt(scriptUpdate);
		Log::Info("Set function pointers! %d", scriptObj);
	}

	void ScriptableComponent::Start()
	{
		m_ScriptStart();
	}

	void ScriptableComponent::Update(float dt)
	{
		m_ScriptUpdate(dt);
	}
}