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

#ifndef QWINDOWSURFACE_TESTLITE_H
#define QWINDOWSURFACE_TESTLITE_H

#include <qwindowsurface_p.h>

QT_BEGIN_NAMESPACE

class QXlibWindow;
class QXlibIntegration;
class QXlibScreen;
class QXlibShmImageInfo;

class QXlibWindowSurface : public QWindowSurface
{
public:
    QXlibWindowSurface (QWidget *window);
    ~QXlibWindowSurface();

    QPaintDevice *paintDevice();
    void flush(QWidget *widget, const QRegion &region, const QPoint &offset);
    bool scroll(const QRegion &area, int dx, int dy);

    void beginPaint(const QRegion &region);
    void endPaint(const QRegion &region);

private:
    bool painted;
    void resizeBuffer(QSize);
    QSize bufferSize() const;


    void resizeShmImage(int width, int height);

    QImage shm_img;
    QXlibShmImageInfo *image_info;

    QXlibWindow *xw;

};


QT_END_NAMESPACE

#endif
