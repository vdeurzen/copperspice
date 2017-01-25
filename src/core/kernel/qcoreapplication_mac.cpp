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

#include <qcoreapplication.h>
#include <qcoreapplication_p.h>
#include <qcore_mac_p.h>

QT_BEGIN_NAMESPACE

/*****************************************************************************
  QCoreApplication utility functions
 *****************************************************************************/
QString qAppFileName()
{
   static QString appFileName;
   if (appFileName.isEmpty()) {
      QCFType<CFURLRef> bundleURL(CFBundleCopyExecutableURL(CFBundleGetMainBundle()));
      if (bundleURL) {
         QCFString cfPath(CFURLCopyFileSystemPath(bundleURL, kCFURLPOSIXPathStyle));
         if (cfPath) {
            appFileName = cfPath;
         }
      }
   }
   return appFileName;
}


QT_END_NAMESPACE
