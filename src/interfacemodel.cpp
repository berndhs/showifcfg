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

  connect (&process, SIGNAL (readyRead()),
           this, SLOT (getData()));
  connect (&process, SIGNAL (finished (int, QProcess::ExitStatus)),
           this, SLOT (doneFinished (int, QProcess::ExitStatus)));
}

QHash<int, QByteArray>
InterfaceModel::roleNames() const
{
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
  Q_UNUSED(ndx);
  return m_data.count();
}

QVariant
InterfaceModel::data(const QModelIndex &index, int role) const
{
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
      break;
    case Data_Info:
      result = QVariant(m_data.at(row).info.join("\n"));
      break;
    case Data_NumLines:
      result = QVariant(m_data.at(row).info.count());
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
  m_data.clear();
}

void
InterfaceModel::addInterface(const QString &name, const QStringList &info, const int fromLine)
{
  beginResetModel();
  ListEntry ntry (name,info) ;
  m_data.append(ntry);
  endResetModel();
}

void
InterfaceModel::read ()
{
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
  nextCmd << cmd;
}

void InterfaceModel::runCommand(const QString &cmd)
{
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
  QByteArray bytes = process.readAll ();
  QString newRaw (QString::fromUtf8(bytes));
  QStringList lines = newRaw.split('\n');
  resultLines.append (lines);
}

void
InterfaceModel::doneFinished (int exitCode, QProcess::ExitStatus exitStatus)
{
  beginResetModel();
  Q_UNUSED (exitCode)
  Q_UNUSED (exitStatus)
  getData ();
  analyze ();

  endResetModel();

}

void
InterfaceModel::analyze()
{
  beginResetModel();
  QStringList nextIF;
  QString     lastIFName;
  for (auto lit=resultLines.begin(); lit != resultLines.end(); ++lit) {
    QString line = *lit;
    if (!line.startsWith(" ")) {
      // new interface
      int leftColon = line.indexOf (":",0);
      int scndColon = line.indexOf (":",leftColon+1);
      if (leftColon >- 0 && scndColon > leftColon) {
        if (!lastIFName.isEmpty()) {
          addInterface(lastIFName,nextIF,__LINE__);
        }
        lastIFName = line.mid(leftColon+1,scndColon-leftColon).trimmed();
        nextIF.clear();
        QString rest = line.mid(scndColon,-1).trimmed();
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
}


} // namespace

