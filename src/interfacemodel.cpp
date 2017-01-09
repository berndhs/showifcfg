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
  roles[Data_IFName] = "name";
  roles[Data_Info] = "info";
  clear();
}

QHash<int, QByteArray>
InterfaceModel::roleNames() const
{
  return roles;
}

int
InterfaceModel::rowCount()
{
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
      result = QVariant(m_data.at(row).info);
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
InterfaceModel::addInterface(const QString &name, const QStringList &info)
{
  beginResetModel();
  ListEntry ntry (name,info);
  m_data.append(ntry);
  endResetModel();
}

} // namespace

