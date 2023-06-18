#pragma once
#include <opencascade/TopoDS_Shape.hxx>
#include <opencascade/BRepOffset_Mode.hxx>
#include <opencascade/GeomAbs_JoinType.hxx>
#include <opencascade/TopExp_Explorer.hxx>

class ShapeBuilder
{
public:
	enum class ShellFaceSelectionMethod {
		MAX_COORD_X,
		MIN_COORD_X,
		MAX_COORD_Y,
		MIN_COORD_Y,
		MAX_COORD_Z,
		MIN_COORD_Z
	};

private:
	static TopoDS_Shape FindFaceByMethod (TopExp_Explorer & faceExplorer, ShellFaceSelectionMethod faceSelectMethod);

public:
	static TopoDS_Shape Bottle(const Standard_Real myWidth, const Standard_Real myHeight, const Standard_Real myThickness);
	static TopoDS_Shape TheShape(bool doFillet, bool showRawShape = false);
	static TopoDS_Shape Cube();
	static TopoDS_Shape Shell(const TopoDS_Shape& originalShape, 
		ShellFaceSelectionMethod faceSelectMethod = ShellFaceSelectionMethod::MAX_COORD_Z,
		Standard_Real thickness = -0.5,
		Standard_Real tolerance = 1.e-3,
		BRepOffset_Mode offsetMode = BRepOffset_Mode::BRepOffset_Skin,
		Standard_Boolean intersection = false,
		GeomAbs_JoinType joinType = GeomAbs_Arc,
		Standard_Boolean removeIntEdges = Standard_False);
};
