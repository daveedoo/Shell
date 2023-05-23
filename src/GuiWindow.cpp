#include "GuiWindow.h"
#include <imgui.h>
#include <opencascade/BRepOffset_Mode.hxx>
#include <opencascade/GeomAbs_JoinType.hxx>

bool GuiWindow::DrawWindow() const
{
    bool changed = false;

    ImGui::Begin("Shell", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);

    static float thickness = this->shellProvider->GetThickness();
    static float tolerance = this->shellProvider->GetTolerance();

    if (ImGui::DragFloat("Thickness", &thickness, 0.01f))
    {
        changed = true;
        this->shellProvider->SetThickness(thickness);
    }
    if (ImGui::DragFloat("Tolerance", &tolerance, 1.e-4f, -FLT_MIN, FLT_MAX, "%.4f"))
    {
        changed = true;
        this->shellProvider->SetTolerance(tolerance);
    }
    
    static const std::string modes[] = { "Skin", "Pipe", "RectoVerse" };
    static BRepOffset_Mode selectedOffsetModeIdx = this->shellProvider->GetOffsetMode();
    if (ImGui::BeginCombo("Offset mode", modes[selectedOffsetModeIdx].c_str()))
    {
        for (size_t i = 0; i < 3; i++)
        {
            if (ImGui::Selectable(modes[i].c_str(), i == selectedOffsetModeIdx))
            {
                changed = true;
                selectedOffsetModeIdx = BRepOffset_Mode(i);
                this->shellProvider->SetOffsetMode(selectedOffsetModeIdx);
            }
        }
        ImGui::EndCombo();
    }

    static const GeomAbs_JoinType joinTypes[] = { GeomAbs_Arc, GeomAbs_Intersection };
    static const std::string joinTypesStr[] = { "Arc", "Intersection" };
    GeomAbs_JoinType selectedJoinType = this->shellProvider->GetJoinType();
    int selectedJoinTypeIdx;
    switch (selectedJoinType)
    {
    case GeomAbs_Arc:
        selectedJoinTypeIdx = 0;
        break;
    case GeomAbs_Intersection:
        selectedJoinTypeIdx = 1;
        break;
    default:
        throw std::exception("Invalid enum GeomAbs_JoinType");
    }

    if (ImGui::BeginCombo("Join type", joinTypesStr[selectedJoinTypeIdx].c_str()))
    {
        for (size_t i = 0; i < 2; i++)
        {
            if (ImGui::Selectable(joinTypesStr[i].c_str(), i == selectedJoinTypeIdx))
            {
                changed = true;
                selectedJoinTypeIdx = i;
                this->shellProvider->SetJoinType(joinTypes[selectedJoinTypeIdx]);
            }
        }
        ImGui::EndCombo();
    }

    static bool removeIntEdges = this->shellProvider->GetRemoveInternalEdges();
    if (ImGui::Checkbox("Remove internal edges", &removeIntEdges))
    {
        changed = true;
        this->shellProvider->SetRemoveInternalEdges(removeIntEdges);
    }

    ImGui::End();

    return changed;
}
