#pragma once

#include "jade/util/Settings.h"

namespace Jade {
    class MenuBar {
    public:
        void ImGui();

    private:
        void SettingsWindow();

    private:
        bool m_SettingsOpen = false;

        // -------------Settings Variables-------------------------
        int m_GridSize[2] = { Settings::General::s_GridSizeX, Settings::General::s_GridSizeY};
        bool m_SnapToGrid = false;
        // ---------------------------------------------------------
    };
}