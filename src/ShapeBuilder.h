#pragma once
#include <opencascade/TopoDS_Shape.hxx>

class ShapeBuilder
{
public:
	static TopoDS_Shape Bottle(const Standard_Real myWidth, const Standard_Real myHeight, const Standard_Real myThickness);
};
