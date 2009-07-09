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


#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QtSql>

#include "media.h"


/** \class DatabaseManager
  *
  */
class DatabaseManager {
    // Constructors
    DatabaseManager();
    ~DatabaseManager();

    // Fields
    static DatabaseManager *singleton_;
    QSqlDatabase db_;


public:
    // Operations
    static DatabaseManager *getInstance();
    static void kill();

    QSqlError init(const QString&);

    QSqlError insertDirToCollection(const QString&);
    QSqlError cleanCollection();
    bool hasDir(const QString&);
    QStringList getCollectionDir();

    void insertMedias(const QList<Media>&);
    QSqlError removeMedias(const QStringList&);
    bool hasMedia(const QString&);
    QStringList queryMediaNames();
    QSqlQuery queryMedias(QSqlQuery&);
};


#endif // DATABASEMANAGER_H
