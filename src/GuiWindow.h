#pragma once
#include <opencascade/TopoDS_Shape.hxx>
#include "ShapeProviders/ShellProvider.h"
#include "TopologyProvider.h"

class GuiWindow
{
private:
	std::shared_ptr<ShellProvider> shellProvider;
	std::unique_ptr<TopologyProvider> topologyProvider;
	bool shapeChanged;

public:
	GuiWindow(std::shared_ptr<ShellProvider> shellProvider)
		: shellProvider(shellProvider), shapeChanged(false) {

		topologyProvider = std::make_unique<TopologyProvider>();
		topologyProvider->AnalyzeShape (shellProvider->GetShape ());
	}

	/// Returns true, if any control changed.
	void DrawWindow();

public:
	bool PopShapeChange ();

private:
	void m_DrawShellOptions ();
	void m_DrawTopologyOptions ();
};