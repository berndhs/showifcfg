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
#include <QProcess>

namespace geuzen
{

class InterfaceModel : public QAbstractItemModel
{
  Q_OBJECT
public:
  InterfaceModel(QObject *parent = Q_NULLPTR);

  Q_PROPERTY(int ifcount READ ifcount NOTIFY ifcountChanged)

  virtual QHash<int, QByteArray> 	roleNames() const;
  virtual int rowCount(const QModelIndex &ndx) const;
  virtual int columnCount(const QModelIndex &ndx) const { return 1; }

  virtual QModelIndex parent( const QModelIndex & duh) const { return duh; }

  Q_INVOKABLE void read ();

  QModelIndex index(int row, int column, const QModelIndex &parent) const;

  QVariant data (const QModelIndex &index, int role) const;
  void clear();

  void addInterface (const QString &name, const QStringList &info, const int fromLine=-1);

  int ifcount() const
  {
    return m_data.count();
  }

public slots:

  void queueCommand (const QString & cmd);
  void runCommand (const QString & cmd = QString());
  void getData ();
  void doneFinished (int exitCode, QProcess::ExitStatus exitStatus);

signals:
  void ifcountChanged(int ifcount);

private:

  void  analyze ();

  enum Data_Type {
    Data_IFName = Qt::UserRole+1,
    Data_Info,
    Data_NumLines
  };


  class ListEntry {
  public:
    QString name;
    QStringList info;
    ListEntry (QString n, QStringList nl) {
      name = n;
      info = nl;
    }
  };

  QHash <int,QByteArray> roles;

  QStringList   nextCmd;
  QStringList   resultLines;

  QList <ListEntry> m_data;
  QProcess      process;

  int m_ifcount;
};

} // namespace

#endif // INTERFACEMODEL_H
