
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
#include <QDeclarativeEngine>
#include <QDeclarativeContext>
#include <QIcon>
#include <QFont>
#include <QSystemDeviceInfo>
#include <qdeclarative.h>
#include "ip-list.h"
#include "qml-text-browser.h"

#include <QDebug>

QTM_USE_NAMESPACE

int
main (int argc, char *argv[])
{
  QApplication app (argc, argv);

  QSystemDeviceInfo sdi;

  QString imsi = sdi.imsi();
  QString imei = sdi.imei();
  bool isPhone (!(imsi.isEmpty() || imei.isEmpty()));

  QDeclarativeView * view = new QDeclarativeView;
  QDeclarativeEngine * engine = view->engine();
  QDeclarativeContext * context = view->rootContext();

  if (context) {
    context->setContextProperty ("isProbablyPhone", QVariant(isPhone));
  }

  if (isPhone) {
    QFont appFont = app.font();
    appFont.setPointSizeF (appFont.pointSizeF() * 2);
    app.setFont (appFont);
  }

  const char uri[] = "moui.geuzen.utils.static";

  qmlRegisterType<geuzen::IpList>(uri, 1, 0, "GeuzenIpList");
  qmlRegisterType<geuzen::QmlTextBrowser>(uri, 1, 0, "GeuzenTextBrowser");

  view->setWindowIcon (QIcon (":/icon.png"));
  view->setSource (QUrl ("qrc:ipaddr.qml"));
  //view->setSource (QUrl::fromLocalFile ("qml/main.qml"));
  if (isPhone) {
    view->setGeometry (0,0,800,480); // won't be full screen on N950 without this
    view->showFullScreen ();         // that is probably a bug in harmattan
  } else {
    view->setGeometry (0,0,500,300);
  }
  qDebug () << " view set to size " << view->size();
  view->setResizeMode (QDeclarativeView::SizeRootObjectToView);

  view->show ();
  QObject::connect (engine, SIGNAL (quit()),&app, SLOT(quit()));
  int ok = app.exec ();
  return ok;
}
