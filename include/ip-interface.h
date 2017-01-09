#ifndef GEUZEN_INTERFACE_H
#define GEUZEN_INTERFACE_H

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
#include <QString>
#include <QStringList>
#include <QObject>
#include <QDebug>
#include <QAbstractListModel>
#include "property.h"

namespace geuzen
{

class NetAddress : public QObject
    // /brief NetAddress is a pair of type and address. Like "inet" and "123.123.100.123"
{
Q_OBJECT
public:

  NetAddress (QObject *parent=0) : QObject (parent) {}
  NetAddress (const NetAddress & other);
  ~NetAddress () {}

  QmlProperty<QString>      type;
  QmlProperty<QString>      address;

  Q_INVOKABLE QString & attribute (unsigned int index) {
    return theAttributes[index];
  }
  Q_INVOKABLE const QString & attribute (unsigned int index) const {
    return theAttributes.at(index);
  }
  Q_INVOKABLE int attributeCount () const {
    return theAttributes.count ();
  }
  Q_INVOKABLE void appendAttribute (const QString & newAttr) {
    theAttributes.append (newAttr);
  }

  Q_INVOKABLE bool isEmpty ();

  Q_INVOKABLE void clear ();

  const QStringList attributes () const {
    qDebug() << Q_FUNC_INFO;
    return theAttributes;
  }

private:

  QStringList    theAttributes;

  friend QDebug operator << (QDebug debug, const NetAddress & address);
};


class NetInterface : public QObject
    // /brief NetInterface is an interface like "wlan0", which can have a bunch of addresses.
{
Q_OBJECT
public:

  NetInterface (QObject *parent=0);
  NetInterface (const NetInterface & other);
  ~NetInterface ();

  QmlProperty <QString>  name;
  QmlProperty <QString>  attributes;

  Q_INVOKABLE NetAddress * address (unsigned int index) {
    return theAddresses[index];
  }
  Q_INVOKABLE const NetAddress * address (unsigned int index) const {
    return theAddresses.at(index);
  }

  Q_INVOKABLE int addressCount () const {
    return theAddresses.count();
  }

  Q_INVOKABLE QString allInfo();

  Q_INVOKABLE void appendAddress (const NetAddress & address);
  Q_INVOKABLE bool isEmpty ();
  Q_INVOKABLE void clear ();

signals:

  void freshData();


private:

  enum Data_Type {
    Type_AddrType = Qt::UserRole+1,
    Type_AddrValue = Qt::UserRole+2,
    Type_AddrAttributes = Qt::UserRole+3
  };

  QList<NetAddress*>  theAddresses;
};

QDebug operator << (QDebug debug, const NetAddress & address);
QDebug operator << (QDebug debug, const NetInterface & iface);

} // namespace


#endif
