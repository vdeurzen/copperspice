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

#ifndef QSQLCACHEDRESULT_P_H
#define QSQLCACHEDRESULT_P_H

#include <QtSql/qsqlresult.h>

QT_BEGIN_NAMESPACE

class QVariant;
template <typename T> class QVector;

class QSqlCachedResultPrivate;

class Q_SQL_EXPORT QSqlCachedResult: public QSqlResult
{
 public:
   virtual ~QSqlCachedResult();

   typedef QVector<QVariant> ValueCache;

 protected:
   QSqlCachedResult(const QSqlDriver *db);

   void init(int colCount);
   void cleanup();
   void clearValues();

   virtual bool gotoNext(ValueCache &values, int index) = 0;

   QVariant data(int i) override;
   bool isNull(int i) override;
   bool fetch(int i) override;
   bool fetchNext() override;
   bool fetchPrevious() override;
   bool fetchFirst() override;
   bool fetchLast() override;

   int colCount() const;
   ValueCache &cache();

   void virtual_hook(int id, void *data) override;

 private:
   bool cacheNext();
   QSqlCachedResultPrivate *d;
};

QT_END_NAMESPACE

#endif // QSQLCACHEDRESULT_P_H
