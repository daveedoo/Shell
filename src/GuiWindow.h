#pragma once
#include <opencascade/TopoDS_Shape.hxx>
#include "ShapeProviders/ShellProvider.h"

class GuiWindow
{
	std::shared_ptr<ShellProvider> shellProvider;

public:
	GuiWindow(std::shared_ptr<ShellProvider> shellProvider)
		: shellProvider(shellProvider) {}

	/// Returns true, if any control changed.
	bool DrawWindow() const;
};