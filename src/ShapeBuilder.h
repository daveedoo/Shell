#pragma once
#include <opencascade/TopoDS_Shape.hxx>
#include <opencascade/BRepOffset_Mode.hxx>
#include <opencascade/GeomAbs_JoinType.hxx>

class ShapeBuilder
{
public:
	static TopoDS_Shape Bottle(const Standard_Real myWidth, const Standard_Real myHeight, const Standard_Real myThickness);
	static TopoDS_Shape TheShape(bool doFillet, bool showRawShape = false);
	static TopoDS_Shape Shell(const TopoDS_Shape& originalShape, Standard_Real thickness = -0.5,
		Standard_Real tolerance = 1.e-3,
		BRepOffset_Mode offsetMode = BRepOffset_Mode::BRepOffset_Skin,
		GeomAbs_JoinType joinType = GeomAbs_Arc,
		Standard_Boolean removeIntEdges = Standard_False);
};
