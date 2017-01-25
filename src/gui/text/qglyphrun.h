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

#ifndef QGLYPHRUN_H
#define QGLYPHRUN_H

#include <QtCore/qsharedpointer.h>
#include <QtCore/qvector.h>
#include <QtCore/qpoint.h>
#include <QtGui/qrawfont.h>

#if !defined(QT_NO_RAWFONT)

QT_BEGIN_NAMESPACE

class QGlyphRunPrivate;

class Q_GUI_EXPORT QGlyphRun
{

 public:
   QGlyphRun();
   QGlyphRun(const QGlyphRun &other);
   ~QGlyphRun();

   QRawFont rawFont() const;
   void setRawFont(const QRawFont &rawFont);

   void setRawData(const quint32 *glyphIndexArray, const QPointF *glyphPositionArray, int size);

   QVector<quint32> glyphIndexes() const;
   void setGlyphIndexes(const QVector<quint32> &glyphIndexes);

   QVector<QPointF> positions() const;
   void setPositions(const QVector<QPointF> &positions);

   void clear();

   QGlyphRun &operator=(const QGlyphRun &other);

   bool operator==(const QGlyphRun &other) const;
   inline bool operator!=(const QGlyphRun &other) const {
      return !operator==(other);
   }

   void setOverline(bool overline);
   bool overline() const;

   void setUnderline(bool underline);
   bool underline() const;

   void setStrikeOut(bool strikeOut);
   bool strikeOut() const;

 private:
   friend class QGlyphRunPrivate;
   friend class QTextLine;

   QGlyphRun operator+(const QGlyphRun &other) const;
   QGlyphRun &operator+=(const QGlyphRun &other);

   void detach();
   QExplicitlySharedDataPointer<QGlyphRunPrivate> d;
};

QT_END_NAMESPACE

#endif // QT_NO_RAWFONT

#endif // QGLYPHS_H
