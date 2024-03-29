#include "ShellProvider.h"
#include "../ShapeBuilder.h"

void ShellProvider::UpdateShell()
{
	if (this->performShell) {
		this->shell = ShapeBuilder::Shell (originalShape,
			this->faceSelectMethod,
			this->thickness,
			this->tolerance,
			this->offsetMode,
			this->intersection,
			this->joinType,
			this->removeIntEdges);
	} else {
		this->shell = originalShape;
	}
}

ShellProvider::ShellProvider(const TopoDS_Shape& originalShape)
	: originalShape(originalShape)
{
	UpdateShell();
}

void ShellProvider::SetPerformShell (bool perform) {
	this->performShell = perform;
	UpdateShell ();
}

void ShellProvider::SetSelectionMethod (ShapeBuilder::ShellFaceSelectionMethod method) {
	this->faceSelectMethod = method;
	UpdateShell ();
}

void ShellProvider::SetThickness(Standard_Real thickness)
{
	this->thickness = thickness;
	UpdateShell();
}

void ShellProvider::SetTolerance(Standard_Real tolerance)
{
	this->tolerance = tolerance;
	UpdateShell();
}

void ShellProvider::SetOffsetMode(BRepOffset_Mode offsetMode)
{
	this->offsetMode = offsetMode;
	UpdateShell();
}

void ShellProvider::SetIntersection(Standard_Boolean intersection)
{
	this->intersection = intersection;
	UpdateShell();
}

void ShellProvider::SetJoinType(GeomAbs_JoinType joinType)
{
	this->joinType = joinType;
	UpdateShell();
}

void ShellProvider::SetRemoveInternalEdges(Standard_Boolean removeIntEdges)
{
	this->removeIntEdges = removeIntEdges;
	UpdateShell();
}
