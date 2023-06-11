#include "GlfwOcctView.h"
#include "ShapeBuilder.h"
#include "GuiWindow.h"
#include "StepIgesModelReader.h"
#include <opencascade/BRepTools.hxx>
#include <opencascade/BRep_Builder.hxx>

int main()
{
    //auto extension = StepIgesModelReader::FileExtension::IGES; Standard_CString file = "res/Assem1.IGS";
    //auto extension = StepIgesModelReader::FileExtension::IGES; Standard_CString file = "res/Assemblage2.IGS";

    auto extension = StepIgesModelReader::FileExtension::STEP; Standard_CString file = "res/knob.stp";
    //auto extension = StepIgesModelReader::FileExtension::STEP; Standard_CString file = "res/LawnMower.stp";
    //auto extension = StepIgesModelReader::FileExtension::STEP; Standard_CString file = "res/MiniPitcher.stp";

    //auto originalShape = StepIgesModelReader::ReadFile(file, extension);
    //auto originalShape = ShapeBuilder::Bottle(50.0, 70.0, 20.0);
    TopoDS_Shape originalShape; BRepTools::Read(originalShape, "res/file.brep", BRep_Builder());

    auto shellProvider = std::make_shared<ShellProvider>(originalShape);

    auto gui = std::make_unique<GuiWindow>(shellProvider);
    GlfwOcctView anApp(std::move(shellProvider), std::move(gui));

    anApp.run();
}
