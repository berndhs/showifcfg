#include "qmlrunner.h"
#include <QTimer>
#include <QUrl>
#include <QQuickItem>

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
  m_viewer->setGeometry (0,0,100,100);
  m_viewer->setSource (QUrl ("qrc:qml/ipaddr.qml"));
}

void
QmlRunner::run()
{
  m_viewer->setSource (QUrl ("qrc:ipaddr.qml"));
  QQuickItem *root = m_viewer->rootObject();
  QObject *box = root->findChild<QObject*>("MainBox");
  qDebug() << Q_FUNC_INFO << root << box;
    qDebug() << Q_FUNC_INFO << "Box wdth" << root->property("mainWidth");

    QVariant vwid = root->property("mainWidth");
    QVariant vhi = root->property("mainHeight");
    QVariant dense = root->property("pixPerMM");
    qDebug() << Q_FUNC_INFO<< "w" << vwid << "h" << vhi << dense;
    double mainWidth = vwid.toDouble();
    double mainHeight = vhi.toDouble();
    qDebug() << Q_FUNC_INFO << "width" << mainWidth << "height" << mainHeight;
    m_viewer->setGeometry (0,0,mainWidth,mainHeight);
    qDebug() << Q_FUNC_INFO << m_viewer->geometry();
}

} // namespace
