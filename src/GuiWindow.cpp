#include "GuiWindow.h"
#include <imgui.h>

bool GuiWindow::DrawWindow() const
{
    bool changed = false;

    ImGui::Begin("Shell", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);

    static float thickness = -0.5f;
    if (changed = ImGui::DragFloat("Thickness", &thickness, 0.01f, -1.f))
    {
        this->shellProvider->SetThickness(thickness);
    }

    ImGui::End();

    return changed;
}
