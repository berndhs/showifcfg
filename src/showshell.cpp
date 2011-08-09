#include "showshell.h"
#include <QDebug>

ShowShell::ShowShell (QObject *parent)
  :QObject (parent),
   process (this),
   refreshTimer (this)
{
  connect (&process, SIGNAL (readyRead()),
           this, SLOT (readOutput()));
  connect (&refreshTimer, SIGNAL (timeout()),
           this, SLOT (runShell()));
}

void
ShowShell::startTimer (int secs)
{
  refreshTimer.start (1000 * secs);
}

void
ShowShell::stopTimer ()
{
  refreshTimer.stop ();
}

void
ShowShell::runShell ()
{
  process.start ("ifconfig");
  qDebug () << " started shell command ";
}

void
ShowShell::readOutput ()
{
  QByteArray newdata = process.readAll ();
  emit newData (QString::fromUtf8 (newdata));
}
