#include "GlfwOcctView.h"
#include "ShapeBuilder.h"
#include "GuiWindow.h"
#include "StepIgesModelReader.h"

int main()
{
    //auto extension = StepIgesModelReader::FileExtension::IGES;
    //Standard_CString file = "res/Assem1.IGS";
    //Standard_CString file = "res/Assemblage2.IGS";

    auto extension = StepIgesModelReader::FileExtension::STEP;
    //Standard_CString file = "res/knob.stp";
    //Standard_CString file = "res/LawnMower.stp";
    Standard_CString file = "res/MiniPitcher.stp";

    auto originalShape = StepIgesModelReader::ReadFile(file, extension);
    //auto originalShape = ShapeBuilder::Bottle(50.0, 70.0, 20.0);
    //auto originalShape = ShapeBuilder::TheShape(false);

    auto shellProvider = std::make_shared<ShellProvider>(originalShape);
    auto gui = std::make_unique<GuiWindow>(shellProvider);
    GlfwOcctView anApp(std::move(shellProvider), std::move(gui));

    anApp.run();
}
