#include "GuiWindow.h"
#include <imgui.h>
#include <opencascade/BRepOffset_Mode.hxx>
#include <opencascade/GeomAbs_JoinType.hxx>

void GuiWindow::DrawWindow()
{
    ImGui::Begin("Shell", (bool*)0, ImGuiWindowFlags_AlwaysAutoResize);

    if (ImGui::CollapsingHeader ("Shell Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
        this->m_DrawShellOptions ();
        ImGui::NewLine ();
    }

    if (ImGui::CollapsingHeader ("Topology Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
        this->m_DrawTopologyOptions ();
        ImGui::NewLine ();
    }

    ImGui::End();
}

bool GuiWindow::PopShapeChange () {
    auto val = shapeChanged;
    shapeChanged = false;

    return val;
}

void GuiWindow::m_DrawShellOptions () {
    bool changed = false;

    static float thickness = this->shellProvider->GetThickness ();
    static float tolerance = this->shellProvider->GetTolerance ();
    static bool performShell = this->shellProvider->GetPerformShell ();

    if (ImGui::Checkbox ("Enable Shell", &performShell)) {
        changed = true;
        this->shellProvider->SetPerformShell (performShell);
    }

    if (ImGui::DragFloat("Thickness", &thickness, 0.01f))
    {
        changed = true;
        this->shellProvider->SetThickness (thickness);
    }
    if (ImGui::DragFloat ("Tolerance", &tolerance, 1.e-4f, -FLT_MIN, FLT_MAX, "%.4f")) {
        changed = true;
        this->shellProvider->SetTolerance (tolerance);
    }

    constexpr const char * faceSelectModes[] = { "Max X", "Min X", "Max Y", "Min Y", "Max Z", "Min Z"};
    static int selectedMode = 4;
    if (ImGui::Combo ("Face Select Mode", &selectedMode, faceSelectModes, 6)) {
        switch (selectedMode) {
            case 0: this->shellProvider->SetSelectionMethod (ShapeBuilder::ShellFaceSelectionMethod::MAX_COORD_X); break;
            case 1: this->shellProvider->SetSelectionMethod (ShapeBuilder::ShellFaceSelectionMethod::MIN_COORD_X); break;
            case 2: this->shellProvider->SetSelectionMethod (ShapeBuilder::ShellFaceSelectionMethod::MAX_COORD_Y); break;
            case 3: this->shellProvider->SetSelectionMethod (ShapeBuilder::ShellFaceSelectionMethod::MIN_COORD_Y); break;
            case 4: this->shellProvider->SetSelectionMethod (ShapeBuilder::ShellFaceSelectionMethod::MAX_COORD_Z); break;
            case 5: this->shellProvider->SetSelectionMethod (ShapeBuilder::ShellFaceSelectionMethod::MIN_COORD_Z); break;
        }

        changed = true;
    }

    static const std::string modes[] = { "Skin", "Pipe", "RectoVerse" };
    static BRepOffset_Mode selectedOffsetModeIdx = this->shellProvider->GetOffsetMode ();
    if (ImGui::BeginCombo ("Offset mode", modes[selectedOffsetModeIdx].c_str ())) {
        for (size_t i = 0; i < 3; i++) {
            if (ImGui::Selectable (modes[i].c_str (), i == selectedOffsetModeIdx)) {
                changed = true;
                selectedOffsetModeIdx = BRepOffset_Mode (i);
                this->shellProvider->SetOffsetMode (selectedOffsetModeIdx);
            }
        }
        ImGui::EndCombo ();
    }

    static const GeomAbs_JoinType joinTypes[] = { GeomAbs_Arc, GeomAbs_Intersection };
    static const std::string joinTypesStr[] = { "Arc", "Intersection" };
    GeomAbs_JoinType selectedJoinType = this->shellProvider->GetJoinType ();
    int selectedJoinTypeIdx;
    switch (selectedJoinType) {
    case GeomAbs_Arc:
        selectedJoinTypeIdx = 0;
        break;
    case GeomAbs_Intersection:
        selectedJoinTypeIdx = 1;
        break;
    default:
        throw std::exception ("Invalid enum GeomAbs_JoinType");
    }

    if (ImGui::BeginCombo ("Join type", joinTypesStr[selectedJoinTypeIdx].c_str ())) {
        for (size_t i = 0; i < 2; i++) {
            if (ImGui::Selectable (joinTypesStr[i].c_str (), i == selectedJoinTypeIdx)) {
                changed = true;
                selectedJoinTypeIdx = i;
                this->shellProvider->SetJoinType (joinTypes[selectedJoinTypeIdx]);
            }
        }
        ImGui::EndCombo ();
    }

    static bool removeIntEdges = this->shellProvider->GetRemoveInternalEdges ();
    if (ImGui::Checkbox ("Remove internal edges", &removeIntEdges)) {
        changed = true;
        this->shellProvider->SetRemoveInternalEdges (removeIntEdges);
    }

    if (changed) {
        topologyProvider->AnalyzeShape (shellProvider->GetShape ());
        this->shapeChanged = true;
    }
}

void GuiWindow::m_DrawTopologyOptions () {
    if (ImGui::BeginListBox ("Faces", ImVec2 (300.0f, 400.0f))) {
        for (const auto & faceData : topologyProvider->GetFaceData ()) {
            std::stringstream ss;
            ss << "face type: " << faceData.typeName << ",\n mass center: (" 
                << faceData.massCenter.X () << "," << faceData.massCenter.Y () << "," << faceData.massCenter.Z () << ")";

            ImGui::Text (ss.str ().c_str ());
        }

        ImGui::EndListBox ();
    }
}
