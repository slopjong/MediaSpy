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

#include "databasemanager.h"


/** \class Collection
  * \brief The Collection is the list of directories selected by the user.
  * Media files present in these directories and their sub-directories are collected to fill the MediaCollection.
  */
class Collection {
public:
    // Fields

    // Constructors
    Collection();
    ~Collection();

    // Accessor Methods
    QStringList getDirList() const;
    void setDirDatabase(const QStringList&);
    void initDirList(const QStringList&);
    void setNDir(const int);
    int getNDir() const;

    QString getDirAt(const int) const;

    // Operations
    QStringList buildFileList();
    QStringList ScanRecDir(const QString&);
    void init();


private:
    // Fields
    QStringList dirList_;
    DatabaseManager* databaseManager_;
    int nDir_;

};

#endif // COLLECTION_H
