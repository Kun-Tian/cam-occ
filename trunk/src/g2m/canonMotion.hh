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
#ifndef CANONMOTION_HH
#define CANONMOTION_HH

#include <string>
#include <vector>
#include <cmath>
#include <limits.h>

#include <TopoDS_Edge.hxx>
#include <TopoDS_Shape.hxx>

#include "canonLine.hh"

//for STRAIGHT_TRAVERSE, STRAIGHT_FEED, ARC_FEED

typedef enum { HELICAL, LINEAR, TRAVERSE } MOTION_TYPE;
/**
\class canonMotion
\brief This class is for the canonical commands STRAIGHT_TRAVERSE, STRAIGHT_FEED, and ARC_FEED.
canonMotion is an ABC. Its children should only be instantiated via canonLine::canonLineFactory(), which creates linearMotion objects for STRAIGHT_TRAVERSE and STRAIGHT_FEED commands, and helicalMotion objects for ARC_FEED commands
*/
class canonMotion: protected canonLine {
  public:
    virtual MOTION_TYPE getMotionType() = 0;
    virtual const TopoDS_Solid& getSolid() = 0;
    bool isThisMotion() {return true;};
    bool isVolumeSuspect() {return sweepIsSuspect;};
    const TopoDS_Solid& bruteForceSweep(); //sweep using brute force, i.e. fuse a solid many times
  protected:
    canonMotion(std::string canonL, machineStatus prevStatus);
    const TopoDS_Shape& mySolid;
    //const TopoDS_Edge& edge; use myUnSolid
    gp_Ax1 getPoseFromCmd();
    bool sweepIsSuspect;
};

#endif //CANONMOTION_HH
