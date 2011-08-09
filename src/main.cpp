#include <QApplication>
#include <QDeclarativeView>
#include <QDeclarativeContext>
#include "showshell.h"
#include <qdeclarative.h>

int
main (int argc, char *argv[])
{
  QApplication app (argc, argv);


  QDeclarativeView * view = new QDeclarativeView;

  qmlRegisterType<ShowShell> ("net.sf.moui",1,0,"ShowShell");

  view->setSource (QUrl ("qrc:main.qml"));
  //view->setSource (QUrl::fromLocalFile ("qml/main.qml"));
  view->setGeometry (0,0,500,300);
  view->setResizeMode (QDeclarativeView::SizeRootObjectToView);

  view->show ();
  return app.exec ();
}