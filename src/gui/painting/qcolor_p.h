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

#ifndef QCOLOR_P_H
#define QCOLOR_P_H

#include <QtCore/qglobal.h>
#include <QtGui/qrgb.h>
#include <QtCore/qstringlist.h>

QT_BEGIN_NAMESPACE

uint qt_get_rgb_val(const char *name);
bool qt_get_named_rgb(const char *, QRgb *);
bool qt_get_named_rgb(const QChar *, int len, QRgb *);
bool qt_get_hex_rgb(const char *, QRgb *);
bool qt_get_hex_rgb(const QChar *, int len, QRgb *);
QStringList qt_get_colornames();

QT_END_NAMESPACE

#endif
