/*
 * Copyright (C) 2009 Stéphane Péchard <stephanepechard@gmail.com>
 *
 * This file is part of MediaSpy.
 *
 * MediaSpy is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MediaSpy is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MediaSpy.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef COLLECTION_H
#define COLLECTION_H

#include <QString>
#include <QStringList>
#include <QStandardItemModel>
//#include <QFileInfo>
//#include <QList>
//#include <QDir>

#include "databasemanager.h"


/** \class Collection
  *
  */
class Collection
{
public:
    // Fields

    // Constructors
    Collection();
    ~Collection();

    // Accessor Methods
    QStringList getDirList() const;
    void setDirList(const QStringList&);
    QString getDirAt(const int) const;
    QStandardItemModel* getDirModel() const;

    // Operations
    void addDirectory(const QString&);
    void removeDirectory(const QString&);
    QStringList buildFileList();
    QStringList ScanRecDir(const QString&);


private:
    // Fields
    QStringList dirList_;
    QStandardItemModel* dirListModel_;
    DatabaseManager* databaseManager_;

};

#endif // COLLECTION_H
