#include "tool.hh"
#include <BRepPrimAPI_MakeRevol.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <Handle_Geom_TrimmedCurve.hxx>
#include <GC_MakeArcOfCircle.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
//tool.cc - functions in classes tool, millTool, etc

tool::tool() {
  //isRotaryTool = false;
  valid = false;
  profile.Nullify();
  type = UNDEFINED;
}

millTool::millTool() {
  shape = UNDEF;
  type = ROTARY_TOOL;
  d=0.0;
  l=0.0;
  revol.Nullify();
  isRotaryTool = true;
}

const TopoDS_Solid millTool::getRevol() {
  if (revol.IsNull()) {
    gp_Ax1 vertical(gp_Pnt(0,0,0),gp_Dir(0,0,1));
    BRepPrimAPI_MakeRevol rev(profile,vertical,M_PI,true);
    valid = rev.IsDone();
    if (valid)
      revol = TopoDS::Solid( rev.Shape() );
  }
  return revol;
}

//aptTool::aptTool() {}

ballnoseTool::ballnoseTool(double diameter, double length) {
  double r = diameter/2.0;
  valid = false;
  Handle(Geom_TrimmedCurve) Tc;
  Tc = GC_MakeArcOfCircle (gp_Pnt(r,0,r), gp_Pnt(0,0,0), gp_Pnt(-r,0,r));
  TopoDS_Edge Ec = BRepBuilderAPI_MakeEdge(Tc);
  TopoDS_Edge E1 = BRepBuilderAPI_MakeEdge(gp_Pnt(r,0,r), gp_Pnt(r,0,length));
  TopoDS_Edge E2 = BRepBuilderAPI_MakeEdge(gp_Pnt(-r,0,r), gp_Pnt(-r,0,length));
  TopoDS_Edge E3 = BRepBuilderAPI_MakeEdge(gp_Pnt(-r,0,length), gp_Pnt(r,0,length));
  BRepBuilderAPI_MakeWire wm(Ec,E1,E2,E3);
  if ( wm.IsDone() ) {
    TopoDS_Wire w = wm.Wire();
    if ( w.Closed() ) {
      BRepBuilderAPI_MakeFace f(w);
      if (f.IsDone()) {
	profile = f.Face();
	valid = true;
	shape = BALLNOSE;
      }
    }
  }
}

//latheTool::latheTool() {}