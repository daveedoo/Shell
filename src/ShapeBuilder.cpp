#include "ShapeBuilder.h"
#include <opencascade/Standard_TypeDef.hxx>
#include <opencascade/GC_MakeArcOfCircle.hxx>
#include <opencascade/GC_MakeSegment.hxx>
#include <opencascade/TopoDS_Edge.hxx>
#include <opencascade/TopoDS_Wire.hxx>
#include <opencascade/BRepBuilderAPI_MakeEdge.hxx>
#include <opencascade/BRepBuilderAPI_MakeWire.hxx>
#include <opencascade/gp_Ax1.hxx>
#include <opencascade/BRepBuilderAPI_Transform.hxx>
#include <opencascade/BRepBuilderAPI_MakeFace.hxx>
#include <opencascade/BRepPrimAPI_MakePrism.hxx>
#include <opencascade/BRepFilletAPI_MakeFillet.hxx>
#include <opencascade/TopExp_Explorer.hxx>
#include <opencascade/BRepPrimAPI_MakeCylinder.hxx>
#include <opencascade/BRepAlgoAPI_Fuse.hxx>
#include <opencascade/Geom_Plane.hxx>
#include <opencascade/BRepOffsetAPI_MakeThickSolid.hxx>
#include <opencascade/Geom_CylindricalSurface.hxx>
#include <opencascade/Geom2d_Ellipse.hxx>
#include <opencascade/Geom2d_TrimmedCurve.hxx>
#include <opencascade/GCE2d_MakeSegment.hxx>
#include <opencascade/BRepLib.hxx>
#include <opencascade/BRepOffsetAPI_ThruSections.hxx>
#include <opencascade/BRepAlgoAPI_Cut.hxx>
#include <opencascade/ChFi3d_FilletShape.hxx>
#include <opencascade/TopoDS.hxx>
#include <opencascade/TopExp_Explorer.hxx>

TopoDS_Shape ShapeBuilder::Bottle(const Standard_Real myWidth, const Standard_Real myHeight, const Standard_Real myThickness)
{
    // Profile : Define Support Points
    gp_Pnt aPnt1(-myWidth / 2., 0, 0);
    gp_Pnt aPnt2(-myWidth / 2., -myThickness / 4., 0);
    gp_Pnt aPnt3(0, -myThickness / 2., 0);
    gp_Pnt aPnt4(myWidth / 2., -myThickness / 4., 0);
    gp_Pnt aPnt5(myWidth / 2., 0, 0);

    // Profile : Define the Geometry
    Handle(Geom_TrimmedCurve) anArcOfCircle = GC_MakeArcOfCircle(aPnt2, aPnt3, aPnt4);
    Handle(Geom_TrimmedCurve) aSegment1 = GC_MakeSegment(aPnt1, aPnt2);
    Handle(Geom_TrimmedCurve) aSegment2 = GC_MakeSegment(aPnt4, aPnt5);

    // Profile : Define the Topology
    TopoDS_Edge anEdge1 = BRepBuilderAPI_MakeEdge(aSegment1);
    TopoDS_Edge anEdge2 = BRepBuilderAPI_MakeEdge(anArcOfCircle);
    TopoDS_Edge anEdge3 = BRepBuilderAPI_MakeEdge(aSegment2);
    TopoDS_Wire aWire = BRepBuilderAPI_MakeWire(anEdge1, anEdge2, anEdge3);

    // Complete Profile
    gp_Ax1 xAxis = gp::OX();
    gp_Trsf aTrsf;

    aTrsf.SetMirror(xAxis);
    BRepBuilderAPI_Transform aBRepTrsf(aWire, aTrsf);
    TopoDS_Shape aMirroredShape = aBRepTrsf.Shape();
    TopoDS_Wire aMirroredWire = TopoDS::Wire(aMirroredShape);

    BRepBuilderAPI_MakeWire mkWire;
    mkWire.Add(aWire);
    mkWire.Add(aMirroredWire);
    TopoDS_Wire myWireProfile = mkWire.Wire();

    // Body : Prism the Profile
    TopoDS_Face myFaceProfile = BRepBuilderAPI_MakeFace(myWireProfile);
    gp_Vec aPrismVec(0, 0, myHeight);
    TopoDS_Shape myBody = BRepPrimAPI_MakePrism(myFaceProfile, aPrismVec);

    // Body : Apply Fillets
    BRepFilletAPI_MakeFillet mkFillet(myBody);
    TopExp_Explorer anEdgeExplorer(myBody, TopAbs_EDGE);
    while (anEdgeExplorer.More()) {
        TopoDS_Edge anEdge = TopoDS::Edge(anEdgeExplorer.Current());
        //Add edge to fillet algorithm
        mkFillet.Add(myThickness / 12., anEdge);
        anEdgeExplorer.Next();
    }

    myBody = mkFillet.Shape();

    // Body : Add the Neck
    gp_Pnt neckLocation(0, 0, myHeight);
    gp_Dir neckAxis = gp::DZ();
    gp_Ax2 neckAx2(neckLocation, neckAxis);

    Standard_Real myNeckRadius = myThickness / 4.;
    Standard_Real myNeckHeight = myHeight / 10.;

    BRepPrimAPI_MakeCylinder MKCylinder(neckAx2, myNeckRadius, myNeckHeight);
    TopoDS_Shape myNeck = MKCylinder.Shape();

    myBody = BRepAlgoAPI_Fuse(myBody, myNeck);

    // Body : Create a Hollowed Solid
    TopoDS_Face   faceToRemove;
    Standard_Real zMax = -1;

    for (TopExp_Explorer aFaceExplorer(myBody, TopAbs_FACE); aFaceExplorer.More(); aFaceExplorer.Next()) {
        TopoDS_Face aFace = TopoDS::Face(aFaceExplorer.Current());
        // Check if <aFace> is the top face of the bottle's neck 
        Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);
        if (aSurface->DynamicType() == STANDARD_TYPE(Geom_Plane)) {
            Handle(Geom_Plane) aPlane = Handle(Geom_Plane)::DownCast(aSurface);
            gp_Pnt aPnt = aPlane->Location();
            Standard_Real aZ = aPnt.Z();
            if (aZ > zMax) {
                zMax = aZ;
                faceToRemove = aFace;
            }
        }
    }

    TopTools_ListOfShape facesToRemove;
    facesToRemove.Append(faceToRemove);
    BRepOffsetAPI_MakeThickSolid aSolidMaker;
    aSolidMaker.MakeThickSolidByJoin(myBody, facesToRemove, -myThickness / 50, 1.e-3);
    myBody = aSolidMaker.Shape();
    // Threading : Create Surfaces
    Handle(Geom_CylindricalSurface) aCyl1 = new Geom_CylindricalSurface(neckAx2, myNeckRadius * 0.99);
    Handle(Geom_CylindricalSurface) aCyl2 = new Geom_CylindricalSurface(neckAx2, myNeckRadius * 1.05);

    // Threading : Define 2D Curves
    gp_Pnt2d aPnt(2. * M_PI, myNeckHeight / 2.);
    gp_Dir2d aDir(2. * M_PI, myNeckHeight / 4.);
    gp_Ax2d anAx2d(aPnt, aDir);

    Standard_Real aMajor = 2. * M_PI;
    Standard_Real aMinor = myNeckHeight / 10;

    Handle(Geom2d_Ellipse) anEllipse1 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor);
    Handle(Geom2d_Ellipse) anEllipse2 = new Geom2d_Ellipse(anAx2d, aMajor, aMinor / 4);
    Handle(Geom2d_TrimmedCurve) anArc1 = new Geom2d_TrimmedCurve(anEllipse1, 0, M_PI);
    Handle(Geom2d_TrimmedCurve) anArc2 = new Geom2d_TrimmedCurve(anEllipse2, 0, M_PI);
    gp_Pnt2d anEllipsePnt1 = anEllipse1->Value(0);
    gp_Pnt2d anEllipsePnt2 = anEllipse1->Value(M_PI);

    Handle(Geom2d_TrimmedCurve) aSegment = GCE2d_MakeSegment(anEllipsePnt1, anEllipsePnt2);
    // Threading : Build Edges and Wires
    TopoDS_Edge anEdge1OnSurf1 = BRepBuilderAPI_MakeEdge(anArc1, aCyl1);
    TopoDS_Edge anEdge2OnSurf1 = BRepBuilderAPI_MakeEdge(aSegment, aCyl1);
    TopoDS_Edge anEdge1OnSurf2 = BRepBuilderAPI_MakeEdge(anArc2, aCyl2);
    TopoDS_Edge anEdge2OnSurf2 = BRepBuilderAPI_MakeEdge(aSegment, aCyl2);
    TopoDS_Wire threadingWire1 = BRepBuilderAPI_MakeWire(anEdge1OnSurf1, anEdge2OnSurf1);
    TopoDS_Wire threadingWire2 = BRepBuilderAPI_MakeWire(anEdge1OnSurf2, anEdge2OnSurf2);
    BRepLib::BuildCurves3d(threadingWire1);
    BRepLib::BuildCurves3d(threadingWire2);

    // Create Threading 
    BRepOffsetAPI_ThruSections aTool(Standard_True);
    aTool.AddWire(threadingWire1);
    aTool.AddWire(threadingWire2);
    aTool.CheckCompatibility(Standard_False);

    TopoDS_Shape myThreading = aTool.Shape();

    // Building the Resulting Compound 
    TopoDS_Compound aRes;
    BRep_Builder aBuilder;
    aBuilder.MakeCompound(aRes);
    aBuilder.Add(aRes, myBody);
    aBuilder.Add(aRes, myThreading);

    return aRes;
}

const TopoDS_Shape& cut(TopoDS_Shape shape, TopoDS_Shape knife, int translation[3] /*= [0, 0, 0]*/)
{
    gp_Trsf tf;
    tf.SetTranslation(gp_Vec(translation[0], translation[1], translation[2]));

    TopLoc_Location loc(tf);

    BRepAlgoAPI_Cut c(shape, knife.Moved(loc, false), Message_ProgressRange());
    c.Build(Message_ProgressRange());

    return c.Shape();
}

TopoDS_Shape ShapeBuilder::Shell(bool doFillet, bool showRawShape)
{
    // geometry
    gp_Pnt g_p1(0, 0, 0);
    gp_Pnt g_p2(5, 0, 0);
    gp_Pnt g_p3(5, 5, 0);
    gp_Pnt g_p4(0, 5, 0);
    gp_Pnt g_circleControl(2.5, 6, 0);

    GC_MakeSegment g_edge1(g_p1, g_p2);
    GC_MakeSegment g_edge2(g_p2, g_p3);
    GC_MakeArcOfCircle g_edge3(g_p3, g_circleControl, g_p4);
    GC_MakeSegment g_edge4(g_p4, g_p1);


    // topology
    BRepBuilderAPI_MakeEdge topo_e1(Handle_Geom_Curve(g_edge1.Value().get()));
    BRepBuilderAPI_MakeEdge topo_e2(Handle_Geom_Curve(g_edge2.Value().get()));
    BRepBuilderAPI_MakeEdge topo_e3(Handle_Geom_Curve(g_edge3.Value().get()));
    BRepBuilderAPI_MakeEdge topo_e4(Handle_Geom_Curve(g_edge4.Value().get()));

    BRepBuilderAPI_MakeWire topo_wire(topo_e1.Edge(), topo_e2.Edge(), topo_e3.Edge(), topo_e4.Edge());
    BRepBuilderAPI_MakeFace topo_face(topo_wire.Wire(), false);

    gp_Vec vec(0, 0, 7);
    BRepPrimAPI_MakePrism topo_prism(topo_face.Shape(), vec, false, true);

    // cut a cylinder inside of this shape
    BRepPrimAPI_MakeCylinder cylinder(1, 10);
    int translation[3] = { 2.5, 2.5, 0 };
    auto rawShape = cut(topo_prism.Shape(), cylinder.Shape(), translation);

    // fillet edges
    // "fillet" = rounded edges
    TopoDS_Shape shape;

    // convenience definitions
    ChFi3d_FilletShape filletShape = ChFi3d_Rational;
    TopAbs_ShapeEnum shapeEnumEdge = TopAbs_EDGE;
    TopAbs_ShapeEnum shapeEnumShape = TopAbs_SHAPE;
    TopAbs_ShapeEnum shapeEnumFace = TopAbs_FACE;


    if (doFillet)
    {
        BRepFilletAPI_MakeFillet fillet(rawShape, filletShape);
        TopExp_Explorer edgeExplorer(rawShape, shapeEnumEdge, shapeEnumShape);

        while (edgeExplorer.More())
        {
            auto edge = TopoDS::Edge(edgeExplorer.Current());
            fillet.Add(0.2, edge);

            edgeExplorer.Next();
        }

        shape = fillet.Shape();
    }
    else {
        shape = rawShape;
    }

    if (showRawShape) {
        return shape;
    }

    TopExp_Explorer faceExplorer(shape, shapeEnumFace, shapeEnumShape);

    int maxZ = -100000;
    TopoDS_Shape faceToRemove;  // TODO: check
    for (; faceExplorer.More(); faceExplorer.Next()) {
        auto face = TopoDS::Face(faceExplorer.Current());

        // get face surface and check if it is a plane
        auto surface = BRep_Tool::Surface(face);

        // this is a crude check if this is a plane
        // probably can be done better?
        if (surface.get()->DynamicType().get()->Name() == "Geom_Plane") {
            auto plane = Handle_Geom_Plane((Geom_Plane*)(surface.get())).get();  // TODO: casting?
            auto point = plane->Location();
            auto z = point.Z();

            std::cout << "plane found at z = " << z << std::endl;

            if (z > maxZ) {
                maxZ = z;
                faceToRemove = faceExplorer.Current();
            }
        }
    }

    // remove face and make hollow solid
    TopTools_ListOfShape facesToRemove;
    facesToRemove.Append(faceToRemove);

    BRepOffsetAPI_MakeThickSolid hollowSolid;
    Standard_Real thickness = -0.5;
    Standard_Real tolerance = 1.e-3;

    std::cout << "building thick solid..." << std::endl;

    try
    {
        hollowSolid.MakeThickSolidByJoin(shape, facesToRemove, thickness, tolerance,
            BRepOffset_Mode::BRepOffset_Skin, false, false,
            GeomAbs_JoinType::GeomAbs_Arc, false,
            Message_ProgressRange());
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
    }

    std::cout << "solid done! building shape..." << std::endl;

    TopoDS_Shape result;

    try
    {
        result = hollowSolid.Shape();
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
    }

    return result; 
}
