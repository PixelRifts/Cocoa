#include "AssetWizard.h"
#include "jade/util/Settings.h"
#include "jade/file/IFile.h"

#include <jsonVendor/json.hpp>

namespace Jade
{
	bool AssetWizard::s_IsImporting = false;
	bool AssetWizard::s_FileChosen = false;
	bool AssetWizard::s_CreatingProject = false;
	char AssetWizard::s_TmpFilename[256];
	FileDialogResult AssetWizard::s_FileImporting{};
	AssetType AssetWizard::s_TypeOfImport = AssetType::None;

	void AssetWizard::ImGui()
	{
		if (s_CreatingProject)
		{
			ImGui::Begin("Create New Project");

			ImGui::LabelText("##tmp_projectname", "Project Name:");
			ImGui::InputText("##tmp_filename", s_TmpFilename, 256);
			ImGui::LabelText("##tmp_projectdir", "Project Directory:");
			ImGui::LabelText("##tmp_showfile", "%s", s_FileImporting.filepath.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Choose Directory"))
			{
				IFileDialog::GetOpenFolderName(".", s_FileImporting);
			}

			if (ImGui::Button("Cancel"))
			{
				s_CreatingProject = false;
			}
			ImGui::SameLine();
			if (ImGui::Button("Create"))
			{
				Settings::General::s_WorkingDirectory = s_FileImporting.filepath;
				json j{
					{"WorkingDirectory", s_FileImporting.filepath}
				};
				std::string prjFile = std::string(s_FileImporting.filepath.c_str()) + "/" + std::string(s_TmpFilename) + ".prj";
				IFile::WriteFile(j.dump(4).c_str(), prjFile.c_str());

				json j2{
					{"CurrentProject", prjFile}
				};
				IFile::WriteFile(j2.dump(4).c_str(), Settings::General::s_EditorSaveData.c_str());
				s_CreatingProject = false;
			}
			ImGui::End();
		}

		if (s_IsImporting)
		{
			if (!s_FileChosen)
			{
				ChooseFile();
			}
			else
			{
				switch (s_TypeOfImport)
				{
				case AssetType::Spritesheet:
					ImGuiImportSpritesheet();
					break;
				case AssetType::Sprite:
					ImGuiImportSprite();
					break;
				}
			}
		}
	}

	void AssetWizard::ChooseFile()
	{
		if (IFileDialog::GetOpenFileName(".", s_FileImporting))
		{
			s_FileChosen = true;
			if (std::string("png").compare(s_FileImporting.extension) == 0)
			{
				s_TypeOfImport = AssetType::Sprite;
			}
		}
		else
		{
			s_IsImporting = false;
		}
	}

	void AssetWizard::ImGuiImportSpritesheet()
	{
		Log::Info("Importing spritesheet here...");
		s_IsImporting = false;
	}

	void AssetWizard::ImGuiImportSprite()
	{
		AssetManager::ImportSprite(s_FileImporting.filepath);
		s_IsImporting = false;
	}
}