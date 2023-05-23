#pragma once
#include <opencascade/TopoDS_Shape.hxx>
#include "ShapeProviders/ShellProvider.h"

class GuiWindow
{
private:
	std::shared_ptr<ShellProvider> shellProvider;
	bool shapeChanged;

public:
	GuiWindow(std::shared_ptr<ShellProvider> shellProvider)
		: shellProvider(shellProvider), shapeChanged(false) {}

	/// Returns true, if any control changed.
	void DrawWindow();

public:
	bool PopShapeChange ();

private:
	void m_DrawShellOptions ();
	void m_DrawTopologyOptions ();
};