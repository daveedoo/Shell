#include "ShellProvider.h"
#include "../ShapeBuilder.h"

void ShellProvider::UpdateShell()
{
	this->shell = ShapeBuilder::Shell(originalShape, this->thickness);
}

ShellProvider::ShellProvider(const TopoDS_Shape& originalShape)
	: originalShape(originalShape)
{
	UpdateShell();
}

void ShellProvider::SetThickness(float thickness)
{
	this->thickness = thickness;
	UpdateShell();
}
