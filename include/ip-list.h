#ifndef GEUZEN_IP_LIST_H
#define GEUZEN_IP_LIST_H

/****************************************************************
 * This file is distributed under the following license:
 *
 * Copyright (C) 2011, Bernd Stramm
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

#include <QProcess>
#include <QString>
#include <QStringList>
#include <QObject>
#include <QAbstractListModel>
#include <QHash>
#include <QByteArray>
#include "ip-interface.h"

namespace geuzen 
{

//class QmlTextBrowser;

class IpList : public QAbstractListModel
{
Q_OBJECT
public:

  IpList (QObject *parent=0);
  ~IpList ();

  int rowCount (const QModelIndex & index = QModelIndex()) const;
  QVariant data (const QModelIndex & index, int role = Qt::DisplayRole) const;
  
  Q_INVOKABLE void read ();
  
  Q_INVOKABLE void sayFresh();
  Q_INVOKABLE int count () const;
  Q_INVOKABLE bool isEmpty () const;
  Q_INVOKABLE const NetInterface * interface (unsigned int index) const;

//  Q_INVOKABLE void populateText (int index, QObject * browser, bool visible);

  Q_PROPERTY (QString css READ css WRITE setCss)
  Q_PROPERTY (QString html READ html WRITE setHtml)

  QHash<int, QByteArray> roleNames() const;

  void setCss (const QString & css);
  QString css () const;

  void setHtml (const QString & html);
  QString html () const;

private slots:

  void queueCommand (const QString & cmd);
  void runCommand (const QString & cmd = QString());
  void getData ();
  void doneFinished (int exitCode, QProcess::ExitStatus exitStatus);

signals:

  void freshData();
  void done ();
  void htmlChanged ();


private:

  enum Data_Type {
    Type_Name = Qt::UserRole+1,
    Type_Attributes = Qt::UserRole+2,
    Type_AttribLines =  Qt::UserRole+3
  };

//  void fillBrowser (int ifaceNdx, QmlTextBrowser * browser);

  // parsing functions
  void  analyze ();
  void  parseFirstLine (const QString & line, NetInterface & iface);
  void  addLine (const QString & line, NetInterface & iface);
  void  addAddress (const QString & line, NetInterface & iface);
  void  addAddressAttribute (const QString & line, NetInterface & iface);


  QHash<int, QByteArray> roles;
  QProcess      process;
  QStringList   nextCmd;
  QStringList   resultLines;

  QString       theCss;
  QString       theHtml;

  QList <NetInterface*> interfaces;

};

} //namespace

#endif
