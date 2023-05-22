#pragma once
#include <functional>
#include <opencascade/TopoDS_Shape.hxx>
#include "ShapeProviders/ShellProvider.h"

class GuiWindow
{
	//std::function<void(const TopoDS_Shape&)> onChangeCommand;
	std::shared_ptr<ShellProvider> shellProvider;

public:
	GuiWindow(std::shared_ptr<ShellProvider> shellProvider)
		: shellProvider(shellProvider) {}

	/// Returns true, if any control changed.
	bool DrawWindow() const;
};