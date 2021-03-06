#include "qml-text-browser.h"


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


#include <QDebug>
#include <QTimer>

namespace geuzen
{

QmlTextBrowser::QmlTextBrowser (QQuickItem *prnt)
  :QQuickTextDocument(prnt)
{
  qDebug() << Q_FUNC_INFO << " at " << prnt;
//  setTextInteractionFlags (Qt::TextSelectableByMouse
//                         | Qt::TextSelectableByKeyboard
//                         | Qt::LinksAccessibleByMouse
//                         | Qt::LinksAccessibleByKeyboard);
  setObjectName ("QmlTextBrowser_");
  connect (this, SIGNAL (linkActivated(const QString &)),
           this, SLOT (doActivateLink(const QString &)));
  emit newData();
  pretendNew = new QTimer (this);
  pretendNew->setInterval(2000);
  connect (pretendNew,SIGNAL(triggered()),this,SLOT(newTimerTest()));
}

//QRectF
//QmlTextBrowser::boundingRect () const
//{
//  return QQuickTextDocument::boundingRect ();
//}

int
QmlTextBrowser::zero ()
{
  return 0;
}

//void
//QmlTextBrowser::adjustSize ()
//{
//  QQuickTextDocument::adjustSize ();
//}

bool
QmlTextBrowser::event (QEvent *evt)
{
  return QQuickTextDocument::event (evt);
}

qreal
QmlTextBrowser::getHeight () const
{
  qreal h = 100.0; /*boundingRect().height();*/
  qDebug() << Q_FUNC_INFO << h;
  return h;
}

qreal
QmlTextBrowser::getWidth () const
{
  qreal w = 30.0; /*boundingRect().width();*/
  qDebug() << Q_FUNC_INFO << w;
  return w;
}

QString
QmlTextBrowser::getName () const
{
  return objectName();
}

void
QmlTextBrowser::setName (const QString & name)
{
  setObjectName (QString ("QmlTextBrowser_") + name);
  qDebug() << Q_FUNC_INFO << objectName();
}

QString QmlTextBrowser::getValue() const
{
  return m_value;
}

void QmlTextBrowser::setValue(QString value)
{
  m_value = value;
}


//void
//QmlTextBrowser::setTextWidth (qreal wid)
//{
//  QQuickTextDocument::setTextWidth (wid);
//  emit heightChanged (getHeight());
//  emit widthChanged (getWidth());
//}

//void
//QmlTextBrowser::setHtml (const QString & html)
//{
//  QQuickTextDocument::setHtml (html);
//  emit heightChanged (getHeight());
//}

void
QmlTextBrowser::doActivateLink (const QString & link)
{
  emit activatedLink (link);
}

void
QmlTextBrowser::newTimerTest()
{
  qDebug() << Q_FUNC_INFO;
  int msecs = pretendNew->interval();
  pretendNew->start(msecs);
  emit newData();
}



} // namespace
