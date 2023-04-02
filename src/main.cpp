#include "GlfwOcctView.h"
#include "ShapeBuilder.h"


int main()
{
    auto bottle = ShapeBuilder::Bottle(50.0, 70.0, 20.0);
    
    GlfwOcctView anApp(bottle);
    anApp.run();
}
