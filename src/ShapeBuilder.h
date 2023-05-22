#pragma once
#include <opencascade/TopoDS_Shape.hxx>

class ShapeBuilder
{
public:
	static TopoDS_Shape Bottle(const Standard_Real myWidth, const Standard_Real myHeight, const Standard_Real myThickness);
	static TopoDS_Shape TheShape(bool doFillet, bool showRawShape = false);
	static TopoDS_Shape Shell(const TopoDS_Shape& originalShape, Standard_Real thickness = -0.5);
};
