#pragma once

#include "LevelEditorScene.h"
#include "LevelEditorSystem.h"
#include "Gizmos.h"
#include "Gui/ImGuiExtended.h"
#include "EditorWindows/InspectorWindow.h"

#include "jade/systems/System.h"
#include "jade/renderer/DebugDraw.h"
#include "jade/physics2d/Physics2DSystem.h"
#include "jade/util/JMath.h"
#include "jade/util/Settings.h"
#include "jade/file/JPath.h"
#include "jade/core/AssetManager.h"
#include "FontAwesome.h"
#include "jade/core/Audio.h"
#include "jade/systems/RenderSystem.h"
#include "jade/ScriptingInterop/Native/ScriptRuntime.h"

#include <entt/entt.hpp>
#include <imgui.h>
#include <array>

namespace Jade
{
	void LevelEditorScene::Init()
	{
		LoadDefaultAssets();

		glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 0);
		m_Camera = new Camera(cameraPos);

		Physics2D::SetScene(this);
		Input::SetScene(this);

		InspectorWindow::ClearAllEntities();

		scriptRuntime = new ScriptRuntime();
		m_Systems.emplace_back(std::make_unique<RenderSystem>("Render System", this));
		m_Systems.emplace_back(std::make_unique<LevelEditorSystem>("LevelEditor System", this));
		m_Systems.emplace_back(std::make_unique<Physics2DSystem>("Physics2D System", this));
		m_Systems.emplace_back(std::make_unique<GizmoSystem>("Gizmo System", this));
	}

	void LevelEditorScene::Start()
	{
		for (const auto& system : m_Systems)
		{
			system->Start();
		}
		scriptRuntime->OnSceneStart(*this);
	}

	void LevelEditorScene::Render()
	{
		for (const auto& system : m_Systems)
		{
			system->Render();
		}
	}

	void LevelEditorScene::Update(float dt)
	{
		if (m_IsRunning)
		{
			Physics2D::Get()->Update(dt);
			scriptRuntime->OnSceneUpdate(*this, dt);
		}

		for (const auto& system : m_Systems)
		{
			system->Update(dt);
		}
	}
}