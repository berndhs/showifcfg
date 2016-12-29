#include "ip-list.h"

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

#include "qml-text-browser.h"
#include <QTimer>
#include <QRegExp>
#include <QDebug>

namespace geuzen
{

IpList::IpList (QObject *parent)
  :QAbstractListModel (parent),
   process (this)
{
  roles[Qt::DisplayRole] = "display";
  roles[Qt::DecorationRole] = "decoration";
  roles[Qt::EditRole] = "edit";
  roles[Qt::ToolTipRole] = "tooltip";
  roles[Qt::StatusTipRole] = "statustip";
  roles[Qt::WhatsThisRole] = "whatsthis";
  roles[Type_Name] = "name";
  roles[Type_Attributes] = "attributes";
  connect (&process, SIGNAL (readyRead()),
           this, SLOT (getData()));
  connect (&process, SIGNAL (finished (int, QProcess::ExitStatus)),
           this, SLOT (doneFinished (int, QProcess::ExitStatus)));
}

IpList::~IpList ()
{
  for (auto iit = interfaces.begin(); iit != interfaces.end(); iit++) {
    if (*iit) {
      delete *iit;
    }
  }
}

int
IpList::count () const
{
  qDebug() << Q_FUNC_INFO << interfaces.count();
  return interfaces.count();
}

QString
IpList::css () const
{
  return theCss;
}

void
IpList::setCss (const QString & css)
{
  theCss = css;
}

QString
IpList::html () const
{
  return theHtml;
}

void
IpList::setHtml (const QString & html)
{
  theCss = html;
}

int 
IpList::rowCount (const QModelIndex & index) const
{
  Q_UNUSED (index)
  int nrows = count();

  qDebug() << Q_FUNC_INFO << index << nrows;
  return nrows;
}

QVariant
IpList::data (const QModelIndex & index, int role) const
{
  qDebug() << Q_FUNC_INFO << index << role;
  QVariant retvar (QString ("unknown"));
  int row = index.row ();
  if (0 > row || row >= count() ) {
    return QVariant (QString ("bad row"));
  }
  switch (role) {
    case Type_Name:
      retvar = interface(row)->name();
      break;
    case Type_Attributes:
      retvar = interface(row)->attributes();
      break;
    default:
      break;
  }
  qDebug() << Q_FUNC_INFO << "return" << retvar;
  return retvar;
}


void
IpList::populateText (int index, QObject * browser, bool visible)
{
  qDebug() <<Q_FUNC_INFO << index << browser;
  QmlTextBrowser * qmlBrowser = qobject_cast<QmlTextBrowser*>(browser);
  if (qmlBrowser) {
    if (visible) {
      fillBrowser (index, qmlBrowser);     
    } else {
      qmlBrowser->setHtml ("");
    }
    }
}

QHash<int, QByteArray> IpList::roleNames() const
{
  return roles;
}

void
IpList::fillBrowser (int ifaceNdx, QmlTextBrowser * browser)
{
  if (!browser) {
    return;
  }
  if (ifaceNdx < 0 || ifaceNdx >= interfaces.count()) {
    browser->setHtml ("invalid index");
    return;
  }
  const NetInterface * iface = interface (ifaceNdx);
  QString head ("<head>\n<style type=text/css>\n");
  head.append (theCss);
  head.append ("\n</style>\n</head>\n");
  QString body ("<body>\n");
  body.append ("<table class=\"iplist\">\n");
  for (int i=0; i<iface->addressCount(); i++) {
    body.append ("<tr>\n");
    body.append ("<td>");
    body.append (iface->address(i)->type());
    body.append ("</td><td>");
    body.append (iface->address(i)->address());
    body.append ("</td>\n</tr>\n<tr><td></td><td>");
    body.append (iface->address(i)->attributes().join("<br>\n"));
    body.append ("</td>\n</tr>\n");
  }
  body.append ("</table>\n</body>");
  theHtml = body;
  browser->setHtml (head + body);
}
 
bool
IpList::isEmpty () const
{
  return interfaces.isEmpty ();
}

void
IpList::read ()
{
  nextCmd.clear ();
  beginRemoveRows (QModelIndex(), 0, count()-1);
  interfaces.clear ();
  endRemoveRows ();
  queueCommand ("ip addr");
  QTimer::singleShot (1, this, SLOT (runCommand()));
}

void IpList::sayFresh()
{
  qDebug() << Q_FUNC_INFO << "lie about fresh data";
  emit freshData();
}

const NetInterface *
IpList::interface (unsigned int index) const
{
  return interfaces.at (index);
}

void
IpList::queueCommand (const QString & cmd)
{
  nextCmd << cmd;
}

void
IpList::runCommand (const QString & cmd)
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
IpList::getData ()
{
  QByteArray bytes = process.readAll ();
  QString newRaw (QString::fromUtf8(bytes));
  QStringList lines = newRaw.split('\n');
  qDebug() << " process had " << lines.count() << "lines to say";
  resultLines.append (lines);
  qDebug() << "resultLines now has" << resultLines.count() << "lines";
}

void 
IpList::doneFinished (int exitCode, QProcess::ExitStatus exitStatus)
{
  qDebug() << Q_FUNC_INFO << exitCode << exitStatus;
  Q_UNUSED (exitCode)
  Q_UNUSED (exitStatus)
  getData (); 
  analyze ();
  emit done ();
  QModelIndex topLeft = (QAbstractListModel::index (0,0));
  QModelIndex botRight = QAbstractListModel::index (count()-1, 0);
//  qDebug() << Q_FUNC_INFO << "\n\t\tdata changed from" << topLeft.row() << " to " << botRight.row();
//  for (int r=0;r<count(); ++r) {
//    qDebug() << "\n\t\t" << r;
//    topLeft = this->index(r,0);
//    botRight = this->index(r,1);
//    qDebug() << "\n\t\tchanged " << topLeft << "\n\t\t\t" << botRight;
//    emit dataChanged (topLeft,botRight);
//  }
  qDebug() << "\n\t\tchanged " << topLeft << "\n\t\t\t" << botRight;
  emit dataChanged (topLeft, botRight);
//  revert ();
  emit freshData();
  qDebug() << "after emit fresh";
}

void
IpList::analyze ()
{
  qDebug() << Q_FUNC_INFO;
  interfaces.clear ();
  NetInterface * currentIF = new NetInterface;
  auto lit = resultLines.begin();
  for (;lit != resultLines.end(); ++lit) {
    QString line (*lit);
    if (line.isEmpty()) {
      continue;
    }
    if (!line.startsWith (" ")) { // new interface
      if (!currentIF->isEmpty()) {
        interfaces.append (currentIF); // append previous one
      }
      currentIF = new NetInterface;
      parseFirstLine (line, *currentIF);
    } else {
      addLine (line, *currentIF);
    }
  }
  if (!currentIF->isEmpty()) {
    interfaces.append (currentIF);
  }
  qDebug() << Q_FUNC_INFO << "came up with " << interfaces.count();
}

void
IpList::parseFirstLine (const QString & line, NetInterface & iface)
{
  iface.clear ();
  QStringList parts = line.split (':');
  if (parts.count() < 2) {
    return;
  }
  iface.name = parts.at(1).trimmed();
  parts.removeFirst();
  parts.removeFirst();
  if (!parts.isEmpty()) {
    iface.attributes = parts.join (" ").trimmed();
  }
}

void
IpList::addLine (const QString & line, NetInterface & iface)
{
  int startPos = line.indexOf (QRegExp("\\S"), QString::SkipEmptyParts);
  if (startPos < 5) {
    addAddress (line, iface);
  } else {
    addAddressAttribute (line, iface); 
  }
}

void
IpList::addAddress (const QString & line, NetInterface & iface)
{
  NetAddress addr;
  QStringList parts = line.split (QRegExp ("\\s+"), QString::SkipEmptyParts);
  if (parts.count() < 2) {
    return;
  }
  addr.type = parts.at(0).trimmed();
  addr.address = parts.at(1).trimmed();
  parts.removeFirst();
  parts.removeFirst();
  if (!parts.isEmpty()) {
    addr.appendAttribute (parts.join (" ").trimmed());
  }
  iface.appendAddress (addr);
}

void
IpList::addAddressAttribute (const QString & line, NetInterface & iface)
{
  if (iface.addressCount() < 1) {
    return;
  }
  NetAddress & addr (*iface.address(iface.addressCount() - 1));
  QStringList parts = line.split (QRegExp ("\\s+"));
  addr.appendAttribute (parts.join (" ").trimmed());
}

QDebug
operator << (QDebug debug, const NetAddress & address)
{
  debug.nospace () << "address type " << address.type() 
                   << " value " << address.address() << endl;
  for (int i=0; i<address.attributeCount(); i++) {
    debug.nospace () << "        " << address.attribute(i) << endl;
  }
  return debug.space ();
}

QDebug
operator << (QDebug debug, const NetInterface & iface)
{
  debug.nospace () << endl << "interface " << iface.name()
                   << "  " << iface.attributes () << endl;
  for (int i=0; i<iface.addressCount(); i++) {
    debug << iface.address(i);
  }
  return debug.space();
}

} // namespace
