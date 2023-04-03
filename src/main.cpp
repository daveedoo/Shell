#include "GlfwOcctView.h"
#include "ShapeBuilder.h"


int main()
{
    //auto shape = ShapeBuilder::Bottle(50.0, 70.0, 20.0);
    auto shape = ShapeBuilder::Shell(false);
    
    GlfwOcctView anApp(shape);
    anApp.run();
}
