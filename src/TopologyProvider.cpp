// TO JEST POTRZEBNE DLA OPENCASCADE :)))
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

#include <opencascade/BRep_Tool.hxx>
#include <opencascade/Standard_CString.hxx>
#include <opencascade/TopExp.hxx>
#include <opencascade/TopoDS_Shell.hxx>
#include <opencascade/TopoDS_Solid.hxx>
#include <opencascade/TopTools_IndexedDataMapOfShapeListOfShape.hxx>
#include <opencascade/TopTools_DataMapOfShapeInteger.hxx>
#include <opencascade/TopOpeBRep.hxx>
#include <opencascade/TopOpeBRepBuild_Tools.hxx>
#include <opencascade/TopTools_ListOfShape.hxx>
#include <cassert>

#include <set>
#include <map>
#include "TopologyProvider.h"

TopologyProvider::TopologyProvider () {
	faces.clear ();
}

std::ostream & operator<< (std::ostream & stream, const gp_Pnt & pnt) {
	stream << pnt.X() << ", " << pnt.Y() << ", " << pnt.Z();
	return stream;
}

void TopologyProvider::AnalyzeShape (const TopoDS_Shape & shape) {
	faces.clear ();

	TopTools_IndexedDataMapOfShapeListOfShape shellSolidMap, faceShellMap, edgeFaceMap;
	TopExp::MapShapesAndAncestors (shape, TopAbs_EDGE, TopAbs_FACE, edgeFaceMap);
	TopExp::MapShapesAndAncestors (shape, TopAbs_FACE, TopAbs_SHELL, faceShellMap);
	TopExp::MapShapesAndAncestors (shape, TopAbs_SHELL, TopAbs_SOLID, shellSolidMap);

	// to pozwala identyfikowaæ obiekty z topologii obiektu intami
	TopTools_DataMapOfShapeInteger mapShapeInteger;
	int shapeId = 1;

	TopExp_Explorer edgeExplorer (shape, TopAbs_ShapeEnum::TopAbs_EDGE, TopAbs_ShapeEnum::TopAbs_SHAPE);
	TopExp_Explorer faceExplorer (shape, TopAbs_ShapeEnum::TopAbs_FACE, TopAbs_ShapeEnum::TopAbs_SHAPE);
	TopExp_Explorer shellExplorer (shape, TopAbs_ShapeEnum::TopAbs_SHELL, TopAbs_ShapeEnum::TopAbs_SHAPE);
	TopExp_Explorer solidExplorer (shape, TopAbs_ShapeEnum::TopAbs_SOLID, TopAbs_ShapeEnum::TopAbs_SHAPE);

	/*
	 * drzewko topologii bêdzie wygl¹da³o tak
	 *       
	 *        SOLID
	 *        /   \
	 *     SHELL  ...
	 *     /   \
	 *   FACE FACE
	 */
	
	// macro
	static const auto getMassCenter = [] (const TopoDS_Shape & shape) -> gp_Pnt {
		GProp_GProps massProps;
		BRepGProp::SurfaceProperties (shape, massProps);
		gp_Pnt massCenter = massProps.CentreOfMass ();

		return massCenter;
	};

	std::map<int, gp_Pnt> centers;

	struct PrintableObject {
		int id;
		std::string type;
		gp_Pnt center;
	};

	const auto printObjects = [](const PrintableObject & a, const PrintableObject & b) -> void {
		std::cout << "\"" << a.type << " " << a.id << "\" \"" << b.type << " " << b.id << "\"" << std::endl;
		return;
	};

	for (; solidExplorer.More (); solidExplorer.Next ()) {
		TopoDS_Solid solid = TopoDS::Solid (solidExplorer.Current ());
		auto massCenter = getMassCenter (solid);
		int id = shapeId++;

		//std::cout << "solid " << id << " " << massCenter << std::endl;

		centers.insert ({id, massCenter});
		mapShapeInteger.Bind (solid, id);
	}

	for (; shellExplorer.More (); shellExplorer.Next ()) {
		TopoDS_Shell shell = TopoDS::Shell (shellExplorer.Current ());
		auto massCenter = getMassCenter (shell);
		int id = shapeId++;

		centers.insert ({ id, massCenter });
		mapShapeInteger.Bind (shell, id);
		shells.push_back (ShellInfo{ massCenter, id });

		if (!shellSolidMap.Contains (shell)) {
			std::cout << "shell " << id << " " << massCenter << std::endl;
			continue;
		}

		auto listOfSolid = shellSolidMap.FindFromKey (shell);
		for (const auto & s : listOfSolid) {
			assert (s.ShapeType () == TopAbs_SOLID);
			auto & solid = TopoDS::Solid (s);
			auto solidId = mapShapeInteger.Find (solid);

			printObjects (PrintableObject{solidId, "solid", centers[solidId]}, PrintableObject{id, "shell", massCenter});
			//std::cout << "solid " << solidId << " -> shell " << id << " " << massCenter << std::endl;
		}
	}

	for (; faceExplorer.More (); faceExplorer.Next ()) {
		TopoDS_Face face = TopoDS::Face (faceExplorer.Current ());
		Handle (Geom_Surface) surface = BRep_Tool::Surface (face);
		int id = shapeId++;

		Standard_CString surfaceTypeNameRaw = surface->DynamicType ()->Name ();
		std::string surfaceTypeName (surfaceTypeNameRaw);

		auto massCenter = getMassCenter (face);

		centers.insert ({ id, massCenter });
		mapShapeInteger.Bind (face, id);
		faces.push_back ({ massCenter, surfaceTypeName });

		// shell
		if (!faceShellMap.Contains (face)) {
			std::cout << "face " << id << " " << massCenter << std::endl;
			continue;
		}

		auto listOfShell = faceShellMap.FindFromKey (face);
		for (const auto & s : listOfShell) {
			assert (s.ShapeType () == TopAbs_SHELL);
			auto & shell = TopoDS::Shell (s);
			auto shellId = mapShapeInteger.Find (shell);

			printObjects (PrintableObject{ shellId, "shell", centers[shellId] }, PrintableObject{ id, "face", massCenter });
			//std::cout << "shell " << shellId << " -> face " << id << " " << massCenter << std::endl;
		}
	}

	for (; edgeExplorer.More (); edgeExplorer.Next ()) {
		TopoDS_Edge edge = TopoDS::Edge (edgeExplorer.Current ());
		int id = shapeId++;
		auto massCenter = getMassCenter (edge);

		centers.insert ({ id, massCenter });
		mapShapeInteger.Bind (edge, id);

		// edge
		if (!edgeFaceMap.Contains (edge)) {
			std::cout << "edge " << id << " " << massCenter << std::endl;
			continue;
		}

		auto listOfFace = edgeFaceMap.FindFromKey (edge);
		for (const auto & s : listOfFace) {
			assert (s.ShapeType () == TopAbs_FACE);
			auto & face = TopoDS::Face (s);
			auto faceId = mapShapeInteger.Find (face);

			printObjects (PrintableObject{ faceId, "face", centers[faceId] }, PrintableObject{ id, "edge", massCenter });
			//std::cout << "face " << faceId << " -> edge " << id << " " << massCenter << std::endl;
		}
	}
}
