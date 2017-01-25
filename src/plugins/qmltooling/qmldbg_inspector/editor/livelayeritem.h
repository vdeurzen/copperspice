/***********************************************************************
*
* Copyright (c) 2012-2017 Barbara Geller
* Copyright (c) 2012-2017 Ansel Sermersheim
* Copyright (c) 2012-2016 Digia Plc and/or its subsidiary(-ies).
* Copyright (c) 2008-2012 Nokia Corporation and/or its subsidiary(-ies).
* All rights reserved.
*
* This file is part of CopperSpice.
*
* CopperSpice is free software. You can redistribute it and/or
* modify it under the terms of the GNU Lesser General Public License
* version 2.1 as published by the Free Software Foundation.
*
* CopperSpice is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
*
* <http://www.gnu.org/licenses/>.
*
***********************************************************************/

#ifndef LIVELAYERITEM_H
#define LIVELAYERITEM_H

#include <QtGui/QGraphicsObject>

namespace QmlJSDebugger {

class LiveLayerItem : public QGraphicsObject
{
public:
    LiveLayerItem(QGraphicsScene *scene);
    ~LiveLayerItem();
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
                QWidget *widget = 0);
    QRectF boundingRect() const;
    int type() const;

    QList<QGraphicsItem*> findAllChildItems() const;

protected:
    QList<QGraphicsItem*> findAllChildItems(const QGraphicsItem *item) const;
};

}

#endif // LIVELAYERITEM_H
