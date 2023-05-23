#pragma once
#include <opencascade/TopoDS_Shape.hxx>

class ShapeProvider
{
public:
	virtual const TopoDS_Shape& GetShape() const = 0;
};
