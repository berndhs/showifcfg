#include "ip-interface.h"


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


namespace geuzen
{

NetAddress::NetAddress (const NetAddress & other)
  :QObject (other.parent()),
   type (other.type()),
   address (other.address()),
   theAttributes (other.theAttributes)
{
}

bool
NetAddress::isEmpty ()
{
  return type().isEmpty();
}

void
NetAddress::clear ()
{
  type.clear ();
  address.clear ();
  theAttributes.clear ();
}

NetInterface::NetInterface (QObject *parent)
  :QObject (parent)
{
}

NetInterface::~NetInterface ()
{
  for (auto ait = theAddresses.begin(); ait != theAddresses.end(); ait++) {
    if (*ait) {
      delete *ait;
    }
  }
}

NetInterface::NetInterface (const NetInterface & other)
  :QObject (other.parent()),
   name (other.name()),
   attributes (other.attributes())
{
  // deep copy of theAddresses list
  for (auto ait = other.theAddresses.begin();
       ait != other.theAddresses.end();
       ait++) {
    appendAddress (**ait);
  }
}

bool
NetInterface::isEmpty ()
{
  return name().isEmpty();
}

void
NetInterface::clear ()
{
  name.clear ();
  attributes.clear ();
  theAddresses.clear ();
} 

void
NetInterface::appendAddress (const NetAddress & address)
{
  NetAddress * pAddr = new NetAddress (address);
  pAddr->setParent (this);
  theAddresses.append (pAddr);
}

} //namespace
