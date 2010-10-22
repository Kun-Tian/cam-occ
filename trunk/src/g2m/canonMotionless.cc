/***************************************************************************
*   Copyright (C) 2010 by Mark Pictor                                     *
*   mpictor@gmail.com                                                     *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/
#include "canonMotionless.hh"
#include "machineStatus.hh"
#include "uio.hh"
#include <string>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <AIS_Shape.hxx>

canonMotionless::canonMotionless(std::string canonL, machineStatus prevStatus):canonLine(canonL, prevStatus) {
  match = true;
  handled = true;
  ncEnd = false;
  status.setMotionType(MOTIONLESS);
  status.setEndPose(status.getStartPose());
  solidErrors = false;
  unsolidErrors = false;
  myUnSolid = BRepBuilderAPI_MakeVertex(status.getStartPose().Location());

  //match canonical commands. the string MUST be the complete command name
  //NOTE: cmdMatch ONLY looks at the command part of the line, canonTokens[2].
  if (cmdMatch("COMMENT")) {
    //do nothing
  } else if (cmdMatch("MESSAGE")) {
    size_t a,b;
    a = myLine.find_first_of("\"") + 1;
    b = myLine.find_last_of("\"");
    uio::infoMsg("Message: " + myLine.substr(a,b-a));
  } else if (cmdMatch("STOP_SPINDLE_TURNING")) {
    status.setSpindleStatus(SPINDLE_STATUS(OFF));
  } else if (cmdMatch("START_SPINDLE_CLOCKWISE")) {
    status.setSpindleStatus(SPINDLE_STATUS(CW));
  } else if (cmdMatch("START_SPINDLE_COUNTERCLOCKWISE")) {
    status.setSpindleStatus(SPINDLE_STATUS(CCW));
  } else if (cmdMatch("SET_SPINDLE_SPEED")) {
    status.setSpindleSpeed(tok2d(3));
  } else if (cmdMatch("MIST_ON")) {
    coolantStruct c = status.getCoolant();
    c.mist = true;
    status.setCoolant(c);
  } else if (cmdMatch("MIST_OFF")) {
    coolantStruct c = status.getCoolant();
    c.mist = false;
    status.setCoolant(c);
  } else if (cmdMatch("FLOOD_ON")) {
    coolantStruct c = status.getCoolant();
    c.flood = true;
    status.setCoolant(c);
  } else if (cmdMatch("FLOOD_OFF")) {
    coolantStruct c = status.getCoolant();
    c.flood = false;
    status.setCoolant(c);
  } else if (cmdMatch("DWELL")) {
  //} else if (cmdMatch("FEEDRATE")) {
  } else if (cmdMatch("SET_FEED_RATE")) {
    status.setFeed(tok2d(3));
  } else if (cmdMatch("SET_FEED_REFERENCE")) {
    if (cantok(3).compare("CANON_XYZ") == 0) {
      //this is the standard feed reference, do nothing
    } else {  //SET_FEED_REFERENCE(CANON_WORKPIECE)
      handled = false;
    }
  } else if (cmdMatch("SELECT_TOOL")) {
    //this only tells the machine to reposition the tool carousel, correct? if so it can be ignored
  } else if (cmdMatch("CHANGE_TOOL")) {
    status.setTool(tok2i(3));
  } else if (cmdMatch("USE_TOOL_LENGTH_OFFSET")) {  //don't need to do anything, unless the toolholder/spindle is modeled
  } else if (cmdMatch("SET_ORIGIN_OFFSETS")) {
    if (cantok(3).compare("0.0000") == 0) {
      /*
      ** this is a common canon statement. we are going to hijack it to produce a warning,
      ** because the data we're getting was produced with a format of %.4f or so.
      */
      uio::infoMsg("Warning, input has reduced precision - expected more zeros: \n" + myLine +"\nModel may fail!");
    } else if (canonTokens.size() == 9) {
      if ((tok2d(3)==0) && (tok2d(4)==0) && (tok2d(5)==0) && (tok2d(6)==0) && (tok2d(7)==0) && (tok2d(8)==0)) {
      //do nothing if all zeros, interp issues this when it starts up and it has no effect
      } else {
        handled = false;
      }
    } else {
      handled = false; //because I still don't know what to do if we have the correct data...
    }
  } else if (cmdMatch("USE_LENGTH_UNITS")) {
    handled = false;
  } else if (cmdMatch("ENABLE_FEED_OVERRIDE")) {
    handled = true;
  } else if (cmdMatch("ENABLE_SPEED_OVERRIDE")) {
    handled = true;
  } else if (cmdMatch("SET_MOTION_CONTROL_MODE")) {
    handled = false;
  } else if (cmdMatch("SET_XY_ROTATION")) {
    if (canonTokens.size() == 6) {
      if ((tok2d(3)==0) && (tok2d(4)==0) && (tok2d(5)==0) ) {
        //no rotation. interp issues this when starting up
      } else {
        handled = false;
      }
    }
  } else if (cmdMatch("SET_NAIVECAM_TOLERANCE")) {
    handled = false;
  } else if (cmdMatch("PROGRAM_END")) {
    ncEnd = true;
  } else if (cmdMatch("PROGRAM_STOP")) {
    ncEnd = true;
  } else if (cmdMatch("SELECT_PLANE" )) {
    if (cantok(3).compare("CANON_PLANE_XZ")==0) {
      status.setPlane(CANON_PLANE_XZ);
    } else if (cantok(3).compare("CANON_PLANE_YZ")==0) {
      status.setPlane(CANON_PLANE_YZ);
    } else if (cantok(3).compare("CANON_PLANE_XY")==0) {
      status.setPlane(CANON_PLANE_XY);
    } else {// sanity check
      uio::infoMsg("Error: Failed to detect CANON_PLANE in _"+cantok(3)+"_:\n" + myLine);
    }
  } else match = false;

  if ( !match || !handled ) {
    std::string m;
    if (!handled) {
      m = "Unhandled canonical command ("+cantok(2)+")";
    } else {
      m = "Unknown canonical command ("+cantok(2)+"): " + canonL;
    }
    if (uio::debuggingOn())
      infoMsg(m);
  }
}

void canonMotionless::display() {
  if ((dispMode == THIN) || (dispMode == BEST)) {
    aisShape = new AIS_Shape(myUnSolid);
  } else return;
//  uio::context()->SetMaterial ( aisShape, NoM, Standard_True );
//  uio::context()->SetDisplayMode ( aisShape,Standard_Integer(dMode),Standard_False );
  uio::context()->Display ( aisShape );
}

