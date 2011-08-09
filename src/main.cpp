
/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2011, Bernd Stramm
 *
 *  This program is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  as published by the Free Software Foundation; either version 2
 *  of the License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, 
 *  Boston, MA  02110-1301, USA.
 ****************************************************************/

#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include <QIcon>
#include "showshell.h"
#include <qdeclarative.h>

int
main (int argc, char *argv[])
{
  QApplication app (argc, argv);


  QDeclarativeView * view = new QDeclarativeView;

  qmlRegisterType<ShowShell> ("net.sf.moui",1,0,"ShowShell");

  view->setWindowIcon (QIcon (":/icon.png"));
  view->setSource (QUrl ("qrc:main.qml"));
  //view->setSource (QUrl::fromLocalFile ("qml/main.qml"));
  view->setGeometry (0,0,500,300);
  view->setResizeMode (QDeclarativeView::SizeRootObjectToView);

  view->show ();
  return app.exec ();
}
