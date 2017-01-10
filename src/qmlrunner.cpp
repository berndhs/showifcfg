#include "qmlrunner.h"
#include <QTimer>
#include <QUrl>

namespace geuzen {

QmlRunner::QmlRunner(QWidget *parent) : QMainWindow(parent),
  m_viewer(0),
  m_engine(0)
{

}

void
QmlRunner::init(QQuickView *view, QQmlEngine *engine)
{
  m_engine = engine;
  m_viewer = view;
  m_context = m_viewer->rootContext();
  m_viewer->setResizeMode (QQuickView::SizeRootObjectToView);

  m_context->setContextProperty ("isProbablyPhone", QVariant(false));
  m_context->setContextProperty ("cppifmodel",&m_cppifmodel);
  QIcon icon( QString("qrc:icon.png"));
  setWindowIcon(icon);
  QTimer::singleShot (3000,this,SLOT(run()));
}

void
QmlRunner::run()
{
  m_viewer->setGeometry (0,0,500,300);
  m_viewer->setSource (QUrl ("qrc:ipaddr.qml"));
}

} // namespace
