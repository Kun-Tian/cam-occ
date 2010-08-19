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

#ifndef HELICALMOTION_HH
#define HELICALMOTION_HH

#include <string>
#include <vector>
#include <cmath>
#include <limits.h>

#include <gp_Pnt.hxx>
#include <gp_Dir.hxx>

#include "canonMotion.hh"
#include "canonLine.hh"
#include "machineStatus.hh"

/**
\class helicalMotion
\brief For the canonical command ARC_FEED.
This class handles both planar arcs and helical arcs. Inherits from canonMotion.
*/

class helicalMotion: protected canonMotion {
  friend canonLine * canonLine::canonLineFactory (std::string l, machineStatus s);
  public:
    helicalMotion(std::string canonL, machineStatus prevStatus);
    MOTION_TYPE getMotionType() {return HELICAL;};
    const TopoDS_Solid& getSolid() {return mySolid;};
  private:
    void helix(gp_Pnt start, gp_Pnt end, gp_Pnt c, gp_Dir dir, int rot);
    //void arc(status.getStartPose().Location(), startVec, status.endPose)
    //void arc(gp_Pnt start, gp_Pnt end, gp_Pnt c, gp_Dir dir, int rot);
    void arc(gp_Pnt start, gp_Vec startVec, gp_Pnt end);
};

#endif //HELICALMOTION_HH
