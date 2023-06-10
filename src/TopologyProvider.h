#pragma once
#include <vector>
#include <opencascade/Standard_TypeDef.hxx>
#include <opencascade/GC_MakeArcOfCircle.hxx>
#include <opencascade/GC_MakeSegment.hxx>
#include <opencascade/TopoDS_Edge.hxx>
#include <opencascade/TopoDS_Face.hxx>
#include <opencascade/TopoDS.hxx>
#include <opencascade/TopoDS_Wire.hxx>
#include <opencascade/TopExp_Explorer.hxx>
#include <opencascade/StdFail_NotDone.hxx>
#include <opencascade/BRepGProp.hxx>
#include <opencascade/GProp_GProps.hxx>

class TopologyProvider {
	public:
		struct FaceInfo {
			gp_Pnt massCenter;
			std::string typeName;
			int topologyId;
			int parent;
		};

		struct EdgeInfo {
			gp_Pnt massCenter;
			int topologyId;
			std::vector<int> adjacent;
		};

		struct ShellInfo {
			gp_Pnt massCenter;
			int topologyId;
			int parent;
		};

		struct SolidInfo {
			gp_Pnt massCenter;
			int topologyId;
			int parent;
		};

		TopologyProvider();
		void AnalyzeShape (const TopoDS_Shape & shape);

		const std::vector<FaceInfo> & GetFaceData () { return faces; }

	private:
		std::vector<FaceInfo> faces;
		std::vector<EdgeInfo> edges;
		std::vector<ShellInfo> shells;
		std::vector<SolidInfo> solids;
};