#pragma once
#include "ShapeProvider.h"
#include "../ShapeBuilder.h"

#include <opencascade/BRepOffset_Mode.hxx>
#include <opencascade/GeomAbs_JoinType.hxx>

class ShellProvider : public ShapeProvider
{
private:
	const TopoDS_Shape& originalShape;
	TopoDS_Shape shell;
	Standard_Real thickness = -0.5;
	Standard_Real tolerance = 1.e-3;
	BRepOffset_Mode offsetMode = BRepOffset_Mode::BRepOffset_Skin;
	GeomAbs_JoinType joinType = GeomAbs_Arc;
	Standard_Boolean removeIntEdges = Standard_False;
	ShapeBuilder::ShellFaceSelectionMethod faceSelectMethod = ShapeBuilder::ShellFaceSelectionMethod::MAX_COORD_Z;

	void UpdateShell();

public:
	ShellProvider(const TopoDS_Shape& originalShape);

	ShapeBuilder::ShellFaceSelectionMethod GetSelectionMethod () { return this->faceSelectMethod; }
	Standard_Real GetThickness() const { return this->thickness; }
	Standard_Real GetTolerance() const { return this->tolerance; }
	BRepOffset_Mode GetOffsetMode() const { return this->offsetMode; }
	GeomAbs_JoinType GetJoinType() const { return this->joinType; }
	Standard_Boolean GetRemoveInternalEdges() const { return this->removeIntEdges; }

	void SetSelectionMethod (ShapeBuilder::ShellFaceSelectionMethod method);
	void SetThickness(Standard_Real thickness);
	void SetTolerance(Standard_Real tolerance);
	void SetOffsetMode(BRepOffset_Mode offsetMode);
	void SetJoinType(GeomAbs_JoinType joinType);
	void SetRemoveInternalEdges(Standard_Boolean removeIntEdges);
	virtual const TopoDS_Shape& GetShape() const override { return this->shell; }
	virtual const TopoDS_Shape& GetOriginalShape() const override { return this->originalShape; };
};
