#ifndef QMLRUNNER_H
#define QMLRUNNER_H

#include <QMainWindow>
#include <QQuickView>
#include <QQmlEngine>
#include <QQmlContext>
#include <QIcon>
#include <QFont>
#include "interfacemodel.h"

namespace geuzen {

class QmlRunner : public QMainWindow
{
  Q_OBJECT
public:
  explicit QmlRunner(QWidget *parent = 0);

  void init (QQuickView *view, QQmlEngine *engine);

signals:

public slots:
  void run();

private:

  QQuickView  *m_viewer;
  QQmlEngine  *m_engine;
  QQmlContext *m_context;

  InterfaceModel m_cppifmodel;
};

} // namespace

#endif // QMLRUNNER_H
