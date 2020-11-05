#include "scripting/JImGui.h"
#include "jade/core/Core.h"
#include "gui/ImGuiExtended.h"

#include <mono/metadata/assembly.h>
#include <mono/jit/jit.h>


		extern "C"
		{
			void _UndoableDragFloat3(const char* label, float* floatArray)
			{
				Jade::Log::Info("Imgui in cpp %2.3f %2.3f %2.3f", floatArray[0], floatArray[1], floatArray[2]);
				glm::vec3 vector = glm::vec3(floatArray[0], floatArray[1], floatArray[2]);
				JImGui::UndoableDragFloat3(label, vector);
				floatArray[0] = vector[0];
				floatArray[1] = vector[1];
				floatArray[2] = vector[2];
			}

			void _UndoableDragFloat2(const char* label, float* floatArray)
			{
				Jade::Log::Info("Imgui in cpp %2.3f %2.3f", floatArray[0], floatArray[1]);
				glm::vec2 vector = glm::vec2(floatArray[0], floatArray[1]);
				JImGui::UndoableDragFloat2(label, vector);
				floatArray[0] = vector[0];
				floatArray[1] = vector[1];
			}

			void _UndoableDragFloat(const char* label, float* floatArray)
			{
				Jade::Log::Info("Imgui in cpp %2.3f", floatArray[0]);
				JImGui::UndoableDragFloat(label, *floatArray);
			}
		}
