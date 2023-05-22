#pragma once
#include "ShapeProvider.h"

class ShellProvider : public ShapeProvider
{
private:
	const TopoDS_Shape& originalShape;
	TopoDS_Shape shell;
	float thickness = -0.5f;

	void UpdateShell();

public:
	ShellProvider(const TopoDS_Shape& originalShape);

	void SetThickness(float thickness);
	virtual const TopoDS_Shape& GetShape() const override { return this->shell; }
};
