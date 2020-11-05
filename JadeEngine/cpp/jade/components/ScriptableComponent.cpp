#include "jade/components/ScriptableComponent.h"
#include "jade/util/Log.h"

namespace Jade
{
	extern "C" JADE void InitScript(ScriptableComponent* scriptObj, ScriptStartFnPt scriptStart, ScriptUpdateFnPt scriptUpdate, ScriptImGuiFnPt scriptImGui)
	{
		scriptObj->SetStartFnPt(scriptStart);
		scriptObj->SetUpdateFnPt(scriptUpdate);
		scriptObj->SetImGuiFnPt(scriptImGui);
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

	void ScriptableComponent::ImGui()
	{
		if (m_ScriptImGui)
			m_ScriptImGui();
	}
}