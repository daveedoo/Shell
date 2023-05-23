#include "GlfwOcctView.h"
#include "ShapeBuilder.h"
#include "GuiWindow.h"


int main()
{
    auto originalShape = ShapeBuilder::TheShape(false);
    auto shellProvider = std::make_shared<ShellProvider>(originalShape);

    auto gui = std::make_unique<GuiWindow>(shellProvider);
    GlfwOcctView anApp(std::move(shellProvider), std::move(gui));

    anApp.run();
}
