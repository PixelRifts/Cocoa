#pragma once

#include "gui/ImGuiExtended.h"

#include "jade/components/components.h"
#include "jade/components/Transform.h"
#include "jade/physics2d/Physics2DSystem.h"
#include "jade/components/ScriptableComponent.h"

namespace Jade
{
	class InspectorWindow
	{
	public:
		static void ImGui();
		static void AddEntity(Entity entity);
		static void RemoveEntity(Entity entity);
		static void ClearAllEntities();
		static Entity GetActiveEntity();

		// =====================================================================
		// Basic components
		// =====================================================================
		static void ImGuiTransform(Transform& transform);

		// =====================================================================
		// Renderer components
		// =====================================================================
		static void ImGuiSpriteRenderer(SpriteRenderer& spr);

		// =====================================================================
		// Script components
		// =====================================================================
		static void ImGuiScriptableComponent(ScriptableComponent& script);

		// =====================================================================
		// Physics components
		// =====================================================================
		static void ImGuiRigidbody2D(Rigidbody2D& rb);
		static void ImGuiAABB(AABB& box);
		static void ImGuiBox2D(Box2D& box);
		static void ImGuiCircle(Circle& circle);

	private:
		static void ImGuiAddComponentButton();
		static void ImGuiAddScriptDropdown(Entity& activeEntity);

	private:
		static std::vector<Entity> s_ActiveEntities;
		static bool s_GettingScript;
	};
}