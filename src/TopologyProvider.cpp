#include <opencascade/BRep_Tool.hxx>
#include <opencascade/Standard_CString.hxx>

#include "TopologyProvider.h"

TopologyProvider::TopologyProvider () {
	faces.clear ();
}

void TopologyProvider::AnalyzeShape (const TopoDS_Shape & shape) {
	faces.clear ();

	TopExp_Explorer edgeExplorer (shape, TopAbs_ShapeEnum::TopAbs_EDGE, TopAbs_ShapeEnum::TopAbs_SHAPE);
	TopExp_Explorer faceExplorer (shape, TopAbs_ShapeEnum::TopAbs_FACE, TopAbs_ShapeEnum::TopAbs_SHAPE);

	for (; faceExplorer.More (); faceExplorer.Next ()) {
		TopoDS_Face face = TopoDS::Face (faceExplorer.Current ());
		Handle (Geom_Surface) surface = BRep_Tool::Surface (face);

		Standard_CString surfaceTypeNameRaw = surface->DynamicType()->Name();
		std::string surfaceTypeName (surfaceTypeNameRaw);

		GProp_GProps massProps;
		BRepGProp::SurfaceProperties (face, massProps);
		gp_Pnt massCenter = massProps.CentreOfMass ();

		faces.push_back ({ massCenter, surfaceTypeName });
	}
}
