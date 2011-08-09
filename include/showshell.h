#ifndef SHOW_SHELL_H

#include <QProcess>
#include <QObject>
#include <QTimer>

class ShowShell : public QObject
{
Q_OBJECT
public:

  ShowShell (QObject *parent = 0);

  Q_INVOKABLE void runShell ();
  Q_INVOKABLE void startTimer (int secs);
  Q_INVOKABLE void stopTimer ();

private slots:

  void readOutput ();

signals:

  void newData (const QString & data);

private:

  QProcess        process;
  QTimer          refreshTimer;
};


#endif
