#include "EditorWindows/AssetWindow.h"
#include "Gui/ImGuiExtended.h"
#include "FontAwesome.h"
#include "Util/Settings.h"
#include "JadeEditorApplication.h"

#include "jade/core/AssetManager.h"
#include "jade/file/IFile.h"
#include "jade/file/IFileDialog.h"
#include "jade/file/JPath.h"
#include "jade/util/Settings.h"
#include "jade/core/Application.h"
#include "jade/scenes/Scene.h"

namespace Jade
{
	static void PushDarkButtonColors()
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_FrameBgActive));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImGui::GetStyleColorVec4(ImGuiCol_FrameBgHovered));
		ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyleColorVec4(ImGuiCol_Text));
		ImGui::PushStyleColor(ImGuiCol_TextDisabled, ImGui::GetStyleColorVec4(ImGuiCol_TextDisabled));
	}

	static void PopDarkButtonColors()
	{
		ImGui::PopStyleColor(4);
	}

	AssetWindow::AssetWindow(Scene* scene)
		: m_Scene(scene)
	{
	}

	void AssetWindow::ImGui()
	{
		ImGui::Begin("Assets");
		ShowMenuBar();

		switch (m_CurrentView)
		{
		case AssetView::TextureBrowser:
			ShowTextureBrowser();
			break;
		case AssetView::SceneBrowser:
			ShowSceneBrowser();
			break;
		case AssetView::ScriptBrowser:
			ShowScriptBrowser();
			break;
		default:
			Log::Warning("Unkown asset view: %d", (int)m_CurrentView);
			break;
		}

		ImGui::End();
	}

	void AssetWindow::ShowMenuBar()
	{
		ImGui::BeginGroup();

		std::array<const char*, (int)AssetView::Length> assetViews = { "TextureBrowser", "SceneBrowser", "ScriptBrowser" };
		JImGui::UndoableCombo<AssetView>(m_CurrentView, "Asset View", assetViews.data(), (int)AssetView::Length);
		ImGui::EndGroup();
		ImGui::Separator();
	}

	bool AssetWindow::IconButton(const char* icon, const char* label, const glm::vec2& size)
	{
		PushDarkButtonColors();

		ImGui::BeginGroup();
		ImGui::PushFont(Settings::EditorStyle::s_LargeIconFont);
		bool res = ImGui::Button(icon, ImVec2(size.x, size.y));
		ImGui::PopFont();

		ImVec2 textSize = ImGui::CalcTextSize(label);
		ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(size.x / 2.0f, 0.0f) - ImVec2(textSize.x / 2.0f, 0.0f));
		ImGui::Text(label);
		ImGui::EndGroup();

		PopDarkButtonColors();
		return res;
	}

	bool AssetWindow::ImageButton(Texture* texture, const char* label, const glm::vec2& size)
	{
		PushDarkButtonColors();

		ImGui::BeginGroup();
		ImGui::PushFont(Settings::EditorStyle::s_LargeIconFont);
		bool res = JImGui::ImageButton(*texture, size);
		ImGui::PopFont();

		ImVec2 textSize = ImGui::CalcTextSize(label);
		ImGui::SetCursorPos(ImGui::GetCursorPos() + ImVec2(size.x / 2.0f, 0.0f) - ImVec2(textSize.x / 2.0f, 0.0f));
		ImGui::Text(label);
		ImGui::EndGroup();

		PopDarkButtonColors();
		return res;
	}

	void AssetWindow::ShowTextureBrowser()
	{
		std::vector<std::shared_ptr<Texture>> textures = AssetManager::GetAllAssets<Texture>(AssetManager::GetScene());
		for (auto tex : textures)
		{
			if (tex->IsDefault())
			{
				continue;
			}

			int texResourceId = tex->GetResourceId();
			ImGui::PushID(texResourceId);
			if (ImageButton(tex.get(), tex->GetFilepath().Filename(), m_ButtonSize))
			{
				//m_Scene->SetActiveAsset(std::static_pointer_cast<Asset>(tex));
			}

			if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceAllowNullID))
			{
				ImGui::SetDragDropPayload("TEXTURE_HANDLE_ID", &texResourceId, sizeof(int));        // Set payload to carry the index of our item (could be anything)
				ImageButton(tex.get(), tex->GetFilepath().Filename(), m_ButtonSize);
				ImGui::EndDragDropSource();
			}
			ImGui::SameLine();

			ImGui::PopID();
		}

		if (IconButton(ICON_FA_PLUS, "Add Texture", m_ButtonSize))
		{
			std::string initialPath;
			FileDialogResult result;
			if (IFileDialog::GetOpenFileName(initialPath, result))
			{
				AssetManager::LoadTextureFromFile(JPath(result.filepath));
			}
		}
	}

	void AssetWindow::ShowScriptBrowser()
	{
		auto scriptDir = Settings::General::s_WorkingDirectory + "scripts";
		auto scriptFiles = IFile::GetFilesInDir(scriptDir);
		int scriptCount = 0;
		for (auto script : scriptFiles)
		{
			ImGui::PushID(scriptCount++);
			if (IconButton(ICON_FA_FILE, script.Filename(), m_ButtonSize))
			{
				std::string cmdToRun = std::string("-g ") + std::string(script.Filepath()) + " -r --add " + scriptDir.Filepath();
				Log::Info("%s", cmdToRun.c_str());
				IFile::RunProgram(Settings::EditorVariables::s_CodeEditorExe, cmdToRun);
			}
			ImGui::SameLine();
			ImGui::PopID();
		}

		static bool creatingScript = false;
		if (IconButton(ICON_FA_PLUS, "New Script", m_ButtonSize))
		{
			creatingScript = true;
			ImGui::OpenPopup("new script menu", 1);
		}

		if (ImGui::BeginPopup("new script menu"))
		{
			static char scriptName[128] = {};
			ImGui::InputText("Script Name: ", scriptName, 124);
			if (JImGui::Button("Create"))
			{
				JPath newFilePath = Settings::General::s_WorkingDirectory + "scripts";
				if (!IFile::CopyFile(Settings::EditorVariables::s_DefaultScriptLocation, newFilePath, scriptName))
				{
					Log::Warning("Failed to create script: '%s'", newFilePath.Filepath());
				}
				else
				{
					creatingScript = false;
					ImGui::CloseCurrentPopup();
				}
			}

			ImGui::EndPopup();
		}
	}

	void AssetWindow::ShowSceneBrowser()
	{
		auto sceneFiles = IFile::GetFilesInDir(Settings::General::s_WorkingDirectory + "scenes");
		int sceneCount = 0;
		for (auto scene : sceneFiles)
		{
			ImGui::PushID(sceneCount++);
			if (IconButton(ICON_FA_FILE, scene.Filename(), m_ButtonSize))
			{
				m_Scene->Save(Settings::General::s_CurrentScene);
				m_Scene->Load(scene);
			}
			ImGui::SameLine();
			ImGui::PopID();
		}

		if (IconButton(ICON_FA_PLUS, "New Scene", m_ButtonSize))
		{
			m_Scene->Save(Settings::General::s_CurrentScene);
			m_Scene->Reset();
			char newSceneTitle[32] = "New Scene (";
			snprintf(&newSceneTitle[11], 32 - 11, "%d).jade", sceneCount);
			Settings::General::s_CurrentScene = Settings::General::s_WorkingDirectory + "scenes" + std::string(newSceneTitle);
			m_Scene->Save(Settings::General::s_CurrentScene);
			JadeEditor* editor = static_cast<JadeEditor*>(Application::Get());
			editor->GetEditorLayer()->SaveProject();
		}
	}
}