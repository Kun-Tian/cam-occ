/***************************************************************************
*   Copyright (C) 2010 by Mark Pictor					   *
*   mpictor@gmail.com							   *
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
#include <iostream>


#include <QProcess>
#include <QStringList>
#include <QString>
#include <QTime>
#include <QCoreApplication>
#include <QEventLoop>
#include <QFileDialog>

#include "g2m.hh"
#include "uio.hh"
#include "canonLine.hh"

g2m::g2m() {
  cout << "g2m ctor" << endl;
  QMenu* myMenu = new QMenu("gcode");
  
  QAction* myAction = new QAction ( "Create 3D Model...", this );
  myAction->setShortcut(QString("Ctrl+M"));
  myAction->setStatusTip ( "Load a .ngc file and create a 3d model" );
  connect(myAction,SIGNAL(triggered()),this,SLOT(slotModelFromFile()));
  myMenu->addAction( myAction );
  
  /*
  **	// do next: show segments one at a time
  **	nextAction = new QAction ( "Do next", this );
  **	nextAction->setShortcut(QString("Ctrl+."));
  **	nextAction->setStatusTip ( "Do next" );
  **	connect(nextAction, SIGNAL(triggered()),this,SLOT(myNextMenuItem()));
  **	myMenu->addAction( nextAction );
  */

  uio::mb()->insertMenu(uio::hm(),myMenu);
  cout << "g2m ctor end" << endl;
}

g2m::~g2m(){
}

void g2m::slotModelFromFile()
{
  //bool success;
  //cleanUp();
  uio::hideGrid();
  uio::axoView();
  
  QString file = QFileDialog::getOpenFileName ( uio::window(), "Choose .ngc file", "./ngc-in", "*.ngc" );
  if ( ! file.endsWith(".ngc") ) {
    uio::infoMsg("You must select a file ending with .ngc!");
    return;
  }
  /*
  if (success) {
 cout << "sweeping..." << endl;
 sweep();
 fitAll();
 hasProcessedNgc = true;
} else {
  infoMsg("Interpreter stopped without finding PROGRAM_END.");
  drawSome(-1);
  showWire();
  fitAll();
}
*/
}

void g2m::interpret(QString file) {
  success = false;
  //FIXME: don't hardcode these paths
  QString ipath = "/opt/src/emc2/trunk/";
  QString interp = ipath + "bin/rs274";
  QString iparm = ipath + "configs/sim/sim_mm.var\n";
  QString itool = ipath + "configs/sim/sim_mm.tbl\n";
  QProcess toCanon;
  bool foundEOF;
  toCanon.start(interp,QStringList(file));
  /**************************************************
  Apparently, QProcess::setReadChannel screws with waitForReadyRead() and canReadLine()
  So, we just fly blind and assume that there are no errors when we navigate
  the interp's "menu", and that it requires no delays.
  **************************************************/
  
  //now give the interpreter the data it needs
  toCanon.write("2\n");	//set parameter file
  toCanon.write(iparm.toAscii());
  toCanon.write("3\n");	//set tool table file
  toCanon.write(itool.toAscii());
  //can also use 4 and 5
  
  toCanon.write("1\n"); //start interpreting
  //cout << "stderr: " << (const char*)toCanon.readAllStandardError() << endl;
  
  if (!toCanon.waitForReadyRead(1000) ) {
    if ( toCanon.state() == QProcess::NotRunning ){
      infoMsg("Interpreter died.  Bad tool table " + itool + " ?");
    } else  infoMsg("Interpreter timed out for an unknown reason.");
    cout << "stderr: " << (const char*)toCanon.readAllStandardError() << endl;
    cout << "stdout: " << (const char*)toCanon.readAllStandardOutput() << endl;
    toCanon.close();
    return;
  }
  
  //if readLine is used at the wrong time, it is possible to pick up a line fragment! will canReadLine() fix that?
  qint64 lineLength;
  char line[260];
  uint fails = 0;
  do {
    if (toCanon.canReadLine()) {
      lineLength = toCanon.readLine(line, sizeof(line));
      if (lineLength != -1 ) {
	foundEOF = processCanonLine(line);
      } else {	//shouldn't get here!
	fails++;
	sleepSecond();
    }
  } else {
    fails++;
    sleepSecond();
  }
} while ( (fails < 100) && 
( (toCanon.canReadLine()) || ( toCanon.state() != QProcess::NotRunning ) )  );
//((lineLength > 0) || 	//loop until interp quits and all lines are read.
//toCanon.canReadLine() ||  
success = foundEOF;
return;
}

bool g2m::processCanonLine ( std::string l )
{
  //need a vector to store canonLine in
  //TODO in header file?
  
  //create the object and get its pointer
  canonLine * cl = canonLine::canonLineFactory
  (l,*lineVector.back()->getStatus());
  //store it
  lineVector.push_back(cl);
  return cl->checkErrors();
}

void g2m::sleepSecond() {
  //sleep 1s and process events
  //cout << "SLEEP..." << endl;
  QTime dieTime = QTime::currentTime().addSecs(1);
  while( QTime::currentTime() < dieTime )
    QCoreApplication::processEvents(QEventLoop::ExcludeUserInputEvents, 100);
  return;
}

void g2m::infoMsg(QString s) {
  cout << s.toStdString() << endl;
}