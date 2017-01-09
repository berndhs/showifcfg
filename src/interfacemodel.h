#ifndef INTERFACEMODEL_H
#define INTERFACEMODEL_H

#include <QAbstractItemModel>
#include <QDebug>


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


#include <QVariant>
#include <QString>
#include <QStringList>
#include <QHash>
#include <QByteArray>
#include <QMap>

namespace geuzen
{

class InterfaceModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  InterfaceModel(QObject *parent);

  virtual QHash<int, QByteArray> 	roleNames() const;
  int rowCount();

  QVariant data (const QModelIndex &index, int role) const;
  void clear();

  void addInterface (const QString &name, const QStringList &info);
private:

  enum Data_Type {
    Data_IFName = Qt::UserRole+1,
    Data_Info = Qt::UserRole+2
  };


  class ListEntry {
    QString name;
    QStringList info;
  };

  QHash <int,QByteArray> roles;

  QList <ListEntry> m_data;

};

} // namespace

#endif // INTERFACEMODEL_H
