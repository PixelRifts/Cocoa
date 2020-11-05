#pragma once
#include "jade/core/Core.h"
#include "jade/file/JPath.h"
#include "jade/scripting/ScriptMetadata.h"

namespace Jade
{
	typedef void(*ScriptStartFnPt)();
	typedef void(*ScriptImGuiFnPt)();
	typedef void(*ScriptUpdateFnPt)(float dt);

	class JADE ScriptableComponent
	{
	public:
		ScriptableComponent(std::shared_ptr<ScriptMetadata> metadata)
			: m_Metadata(metadata), m_ScriptStart(nullptr), m_ScriptUpdate(nullptr) {}

		ScriptableComponent(ScriptableComponent&& other) noexcept
			: m_ScriptStart(std::move(other.m_ScriptStart)), m_ScriptUpdate(std::move(other.m_ScriptUpdate)), 
			m_ScriptImGui(std::move(other.m_ScriptImGui)), m_Metadata(std::move(other.m_Metadata))
		{
		}

		void Start();
		void Update(float dt);
		void ImGui();

		void SetStartFnPt(ScriptStartFnPt startFnPt) { m_ScriptStart = startFnPt; }
		void SetUpdateFnPt(ScriptUpdateFnPt updateFnPt) { m_ScriptUpdate = updateFnPt; }
		void SetImGuiFnPt(ScriptImGuiFnPt imguiFnPt) { m_ScriptImGui = imguiFnPt; }

		inline const JPath& GetFilepath() const { return m_Metadata->GetFilepath(); }
		inline std::shared_ptr<ScriptMetadata> GetMetadata() const { return m_Metadata; }

		ScriptableComponent& operator=(ScriptableComponent&& other) noexcept
		{
			m_ScriptStart = std::move(other.m_ScriptStart);
			m_ScriptUpdate = std::move(other.m_ScriptUpdate);
			m_ScriptImGui = std::move(other.m_ScriptImGui);
			m_Metadata = std::move(other.m_Metadata);
			return *this;
		}

	private:
		ScriptStartFnPt m_ScriptStart = nullptr;
		ScriptUpdateFnPt m_ScriptUpdate = nullptr;
		ScriptImGuiFnPt m_ScriptImGui = nullptr;
		std::shared_ptr<ScriptMetadata> m_Metadata;
	};

	extern "C" JADE void InitScript(ScriptableComponent* scriptObj, ScriptStartFnPt scriptStart, ScriptUpdateFnPt scriptUpdate, ScriptImGuiFnPt scriptImGui);
}