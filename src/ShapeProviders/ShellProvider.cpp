#include "ShellProvider.h"
#include "../ShapeBuilder.h"

void ShellProvider::UpdateShell()
{
	this->shell = ShapeBuilder::Shell(originalShape, 
		this->faceSelectMethod,
		this->thickness, 
		this->tolerance,
		this->offsetMode,
		this->joinType,
		this->removeIntEdges);
}

ShellProvider::ShellProvider(const TopoDS_Shape& originalShape)
	: originalShape(originalShape)
{
	UpdateShell();
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
