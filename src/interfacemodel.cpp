#include "interfacemodel.h"

/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2017, Bernd Stramm
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

#include <QTimer>
#include <QDebug>

namespace geuzen {

InterfaceModel::InterfaceModel(QObject *parent)
  :QAbstractItemModel(parent)
{
  qDebug() << Q_FUNC_INFO;
  roles[Qt::DisplayRole] = "display";
  roles[Qt::DecorationRole] = "decoration";
  roles[Qt::EditRole] = "edit";
  roles[Qt::ToolTipRole] = "toolTip";
  roles[Qt::StatusTipRole] = "statusTip";
  roles[Qt::WhatsThisRole] = "whatsThis";
  roles[Data_IFName] = "ifname";
  roles[Data_Info] = "info";
  roles[Data_NumLines] = "numlines";
  clear();
  qDebug() << Q_FUNC_INFO << roles;

  connect (&process, SIGNAL (readyRead()),
           this, SLOT (getData()));
  connect (&process, SIGNAL (finished (int, QProcess::ExitStatus)),
           this, SLOT (doneFinished (int, QProcess::ExitStatus)));
}

QHash<int, QByteArray>
InterfaceModel::roleNames() const
{
  qDebug() << Q_FUNC_INFO;
  return roles;
}

QModelIndex InterfaceModel::index(int row, int column, const QModelIndex &parent) const
{
  QModelIndex newGuy = createIndex(row,column);
  return newGuy;
}

int
InterfaceModel::rowCount(const QModelIndex &ndx) const
{
  qDebug() << Q_FUNC_INFO;
  Q_UNUSED(ndx);
  return m_data.count();
}

QVariant
InterfaceModel::data(const QModelIndex &index, int role) const
{
  qDebug() << Q_FUNC_INFO;
  int row = index.row();
  QVariant result ("Invalid Model Data");
  if (row < 0 || row >= m_data.count()) {
    return result;
  }
  if (role < Data_IFName || role > Data_Info) {
    return result;
  }
  switch (role) {
    case Data_IFName:
      result = QVariant(m_data.at(row).name);
      qDebug() << Q_FUNC_INFO << "ifname" << result;
      break;
    case Data_Info:
      result = QVariant(m_data.at(row).info.join("\n"));
      qDebug() << Q_FUNC_INFO << "info" << result;
      break;
    case Data_NumLines:
      result = QVariant(m_data.at(row).info.count());

      qDebug() << Q_FUNC_INFO << "numlines" << result;
      break;
    default:
      result = QVariant("Bad Data");
      break;
  }
  return result;
}

void
InterfaceModel::clear ()
{
  qDebug() << Q_FUNC_INFO;
  m_data.clear();
}

void
InterfaceModel::addInterface(const QString &name, const QStringList &info, const int fromLine)
{
  qDebug() << Q_FUNC_INFO;
  beginResetModel();
  ListEntry ntry (name,info) ;
  m_data.append(ntry);
  endResetModel();
  qDebug() << Q_FUNC_INFO << "from " << fromLine << "\n" << name << info;
}

void
InterfaceModel::read ()
{
  qDebug() << Q_FUNC_INFO;
  nextCmd.clear ();
  beginRemoveRows (QModelIndex(), 0, m_data.count()-1);
  m_data.clear ();
  endRemoveRows ();
  queueCommand ("ip addr");
  QTimer::singleShot (1, this, SLOT (runCommand()));
}


void
InterfaceModel::queueCommand (const QString & cmd)
{
  qDebug() << Q_FUNC_INFO;
  nextCmd << cmd;
}

void InterfaceModel::runCommand(const QString &cmd)
{
  qDebug() << Q_FUNC_INFO;
  QString doNext (cmd);
  if (doNext.isEmpty()) {
    if (!nextCmd.isEmpty()) {
      doNext = nextCmd.takeAt(0);
    }
  }
  if (!doNext.isEmpty()) {
    resultLines.clear ();
    process.start (doNext);
  }
}
void
InterfaceModel::getData ()
{
  qDebug() << Q_FUNC_INFO;
  QByteArray bytes = process.readAll ();
  QString newRaw (QString::fromUtf8(bytes));
  QStringList lines = newRaw.split('\n');
  resultLines.append (lines);
}

void
InterfaceModel::doneFinished (int exitCode, QProcess::ExitStatus exitStatus)
{
  qDebug() << Q_FUNC_INFO << exitCode << exitStatus;
  beginResetModel();
  Q_UNUSED (exitCode)
  Q_UNUSED (exitStatus)
  getData ();
  qDebug() << Q_FUNC_INFO << __LINE__
           << resultLines;
  analyze ();

  endResetModel();

}

void
InterfaceModel::analyze()
{
  qDebug() << Q_FUNC_INFO;
  beginResetModel();
  QStringList nextIF;
  QString     lastIFName;
  for (auto lit=resultLines.begin(); lit != resultLines.end(); ++lit) {
    QString line = *lit;
    if (!line.startsWith(" ")) {
      // new interface
      int leftColon = line.indexOf (":",0);
      int scndColon = line.indexOf (":",leftColon+1);
      qDebug() << "from " << leftColon << " to " << scndColon;
      if (leftColon >- 0 && scndColon > leftColon) {
        if (!lastIFName.isEmpty()) {
          addInterface(lastIFName,nextIF,__LINE__);
        }
        lastIFName = line.mid(leftColon+1,scndColon-leftColon).trimmed();
        nextIF.clear();
        QString rest = line.mid(scndColon,-1).trimmed();
        qDebug() << "interface" << lastIFName << "rest of line: " << rest;
        nextIF.append (rest);
      } else { // nohing found, just add line
        nextIF.append(line);
      }
    } else {
      nextIF.append(line);
    }
  }
  if (nextIF.count() > 0) {
    addInterface(lastIFName,nextIF,__LINE__);
  }
  endResetModel();
  qDebug() << Q_FUNC_INFO;
}


} // namespace

