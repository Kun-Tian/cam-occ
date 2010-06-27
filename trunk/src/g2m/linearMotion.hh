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
#ifndef LINEARMOTION_HH
#define LINEARMOTION_HH

#include <string>
#include <vector>

#include <cmath>
#include <limits.h>
#include "canonMotion.hh"
#include "machineStatus.hh"

/**
\class linearMotion
\brief For the canonical commands STRAIGHT_FEED and STRAIGHT_TRAVERSE.
This class inherits from canonMotion.
*/

class linearMotion: protected canonMotion {
  public:
    linearMotion(std::string canonL, machineStatus prevStatus);
    MOTION_TYPE getMotionType();
    const TopoDS_Solid& getSolid();
};

#endif //LINEARMOTION_HH
