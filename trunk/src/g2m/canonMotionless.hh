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
#ifndef CANONMOTIONLESS_HH
#define CANONMOTIONLESS_HH

#include <string>
#include <vector>

#include <cmath>
#include <limits.h>

#include "canonLine.hh"
#include "machineStatus.hh"

/**
\class canonMotionless
\brief A canonical command that (generally) does not cause or alter axis motion
This class is for anything other than LINEAR_FEED, LINEAR_TRAVERSE, and ARC_FEED - including changes in feedrate, spindle speed, tool, coolant, ending the program, etc
*/

class canonMotionless: protected canonLine {
  public:
    canonMotionless(std::string canonL, machineStatus prevStatus);
    bool isThisMotion() {return false;};
  protected:
    bool match, handled;
};

#endif //CANONMOTIONLESS_HH
